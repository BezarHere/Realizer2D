#include "global.h"
#include "Engine.h"


_R2D_NAMESPACE_START_

void Engine::Init()
{
	for (int i = 0; i < Keyboard_t::KeyCount; i++)
	{
		s_keyboardInput[(Keyboard_t::Key)i];
	}

	for (int i = 0; i < Mouse_t::ButtonCount; i++)
	{
		s_mouseInput[(Mouse_t::Button)i];
	}


	Main();
	atexit(Engine::Final);
}

Error Engine::Fire()
{
	VisualServer::start();

	Engine::Init();
	return Error::Ok;
}

void Engine::RegisterObject(Object2D* object)
{
	assert_msg(object->getObjectID() == 0, "can't register a registered object");
	ObjID_t current_id = ++s_objCounter;
	object->setObjectID(current_id);
	s_objsRegistery.insert_or_assign(current_id, object);
}

void Engine::PopObject(Object2D* object)
{
	assert_msg(object->getObjectID() != 0, "can't pop an orphan object");
	s_objsRegistery.erase(object->getObjectID());
}

void Engine::SetProcessAction(ProcessFunction_t action)
{
	Engine::s_processAction = action;
}

void Engine::SetPhysicsProcessAction(PhysicsFunction_t action)
{
	Engine::s_physicsAction = action;
}

void Engine::SetOnInitAction(Action_t action)
{
	Engine::s_onInitAction = action;
}

void Engine::Update()
{
	auto& root_objs = SceneTree::GetRootObjects();
	for (Object2D *p : root_objs)
	{
		p->doUpdate(s_physicsDeltaTime);
	}

	if (Engine::s_processAction)
		Engine::s_processAction();
}

void Engine::physics()
{
	if (Engine::s_physicsAction)
		Engine::s_physicsAction(s_physicsDeltaTime);
}

void Engine::draw()
{
	VisualServer::DoDraw();	
}

void Engine::pollEvents()
{
	sf::RenderWindow* MainWindow = VisualServer::GetWindow();
	sf::Event window_event;

	while (MainWindow->pollEvent(window_event)) {

		switch (window_event.type)
		{
			case sf::Event::MouseMoved:
				s_lastMousePosition = s_mousePosition;
				s_mousePosition.x = (float)window_event.mouseMove.x;
				s_mousePosition.y = (float)window_event.mouseMove.y;
				s_worldMousePosition = s_mousePosition + VisualServer::GetScreenOrigin();
				break;
			case sf::Event::KeyPressed:
				registerInput(window_event.key, true);
				break;
			case sf::Event::KeyReleased:
				registerInput(window_event.key, false);
				break;
			case sf::Event::MouseButtonPressed:
				registerInput(window_event.mouseButton, true);
				break;
			case sf::Event::MouseButtonReleased:
				registerInput(window_event.mouseButton, false);
				break;
			case sf::Event::MouseWheelScrolled:
				registerInput(window_event.mouseWheelScroll);
				break;
			case sf::Event::Closed:
				exit(EXIT_SUCCESS);
				break;
			default:
				break;
		}
	}
}

void Engine::Final()
{
	assert_msg(
		Engine::s_mainLoopLocked,
		"You should call 'start' on the main R2D engine signleton to start the program or the program will close immedialty"
	);

}

void Engine::Main()
{
	assert_msg(!s_mainLoopLocked, "already running main loop");
	s_mainLoopLocked = true;

	sf::RenderWindow* MainWindow = VisualServer::GetWindow();
	if (Engine::s_onInitAction)
		Engine::s_onInitAction();
	while (MainWindow->isOpen())
	{
		std::chrono::steady_clock::time_point current_frame_time = std::chrono::high_resolution_clock::now();
		s_physicsDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(current_frame_time - s_lastFrameTime).count() / 1'000'000.0f;
		s_lastFrameTime = current_frame_time;
		s_currentFrame++;

		VisualServer::PreDraw();
		pollEvents();
		Update();
		physics();
		draw();

		SceneTree::FlushDeletionQueue();
	}
}


bool Engine::IsMouseButtonPressed(sf::Mouse::Button button)
{
	return s_mouseInput.at(button).pressed;
}

bool Engine::IsMouseButtonJustPressed(sf::Mouse::Button button)
{
	return s_mouseInput.at(button).pressed && s_mouseInput.at(button).frame == s_currentFrame;
}

bool Engine::IsMouseButtonJustReleased(sf::Mouse::Button button)
{
	return !s_mouseInput.at(button).pressed && s_mouseInput.at(button).frame == s_currentFrame;
}

bool Engine::IsScrollingUp()
{
	return s_mouseWheelV > 0;
}

bool Engine::IsScrollingDown()
{
	return s_mouseWheelV < 0;
}

bool Engine::IsScrollingLeft()
{
	return s_mouseWheelV > 0;
}

bool Engine::IsScrollingRight()
{
	return s_mouseWheelV < 0;
}

bool Engine::IsKeyPressed(sf::Keyboard::Key key)
{
	return s_keyboardInput.at(key).pressed;
}

bool Engine::IsKeyJustPressed(sf::Keyboard::Key key)
{
	return s_keyboardInput.at(key).pressed && s_keyboardInput.at(key).frame == s_currentFrame;
}

bool Engine::IsKeyJustReleased(sf::Keyboard::Key key)
{
	return !s_keyboardInput.at(key).pressed && s_keyboardInput.at(key).frame == s_currentFrame;
}

Vector2 Engine::GetMousePosition()
{
	return s_worldMousePosition;
}

Vector2 Engine::GetMouseScreenPosition()
{
	return s_mousePosition;
}


void Engine::registerInput(const sf::Event::MouseButtonEvent& button_event, bool pressed)
{
	InputEvent& input_event = s_mouseInput[button_event.button];
	input_event.pressed = pressed;
	input_event.frame = s_currentFrame;
}

void Engine::registerInput(const sf::Event::MouseWheelScrollEvent& scroll_event)
{
	if (scroll_event.wheel == sf::Mouse::VerticalWheel)
		s_mouseWheelV = scroll_event.delta;
	else
		s_mouseWheelH = scroll_event.delta;
}

void Engine::registerInput(const sf::Event::KeyEvent& key_event, bool pressed)
{
	InputEvent& input_event = s_keyboardInput.at(key_event.code);
	input_event.pressed = pressed;
	input_event.frame = s_currentFrame;
}

_R2D_NAMESPACE_END_