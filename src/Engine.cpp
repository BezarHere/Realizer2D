#include "global.h"
#include "Engine.h"


_R2D_NAMESPACE_START_

Engine::Engine()
	: m_mouseWheelH(),
		m_mouseWheelV(),
		m_currentFrame(),
		m_keyboardInput(),
		m_mouseInput()
{
	random::randomize();
	assert_msg(
		!Engine::s_instance,
		"r2d::Engine can't be created by the user, use r2d::Engine::GetSingleton()"
	);


	for (int i = 0; i < Keyboard_t::KeyCount; i++)
	{
		m_keyboardInput[(Keyboard_t::Key)i];
	}

	for (int i = 0; i < Mouse_t::ButtonCount; i++)
	{
		m_mouseInput[(Mouse_t::Button)i];
	}

	PEEK((sizeof(Object2D) + sizeof(components::RectangleDrawer)) * 1000);
	//main_loop();
	atexit(Engine::finalize_program);
}

void Engine::Fire()
{
	/*assert_msg(
		!mainEngine,
		"can't fire an already fired engine, use r2d::Engine::GetSingleton() to get the main engine singleton"
	);*/
	VisualServer::start();

	Engine::s_instance = new Engine();
	Engine::s_instance->start();
}

void Engine::RegisterObject(Object2D* object)
{
	assert_msg(object->getObjectID() == 0, "can't register a registered object");
	ObjectID current_id = m_objCounter++;
	object->setObjectID(current_id);
	m_objsRegistery.insert_or_assign(current_id, object);
}

void Engine::start()
{
	main_loop();
}

void Engine::SetProcessAction(ProcessFunction_t action)
{
	Engine::m_processAction = action;
}

void Engine::SetPhysicsProcessAction(PhysicsFunction_t action)
{
	Engine::m_physicsAction = action;
}

void Engine::SetDrawingAction(DrawerFunction_t action)
{
	Engine::m_drawAction = action;
}

void Engine::SetOnInitAction(Action_t action)
{
	Engine::m_onInitAction = action;
}

void Engine::PopObject(Object2D* object)
{
	assert_msg(object->getObjectID() != 0, "can't pop an orphan object");
	m_objsRegistery.erase(object->getObjectID());
}

void Engine::update()
{
	if (Engine::m_processAction)
		Engine::m_processAction();
}

void Engine::physics()
{
	if (Engine::m_physicsAction)
		Engine::m_physicsAction(1.0f / 60.0f);

}

void Engine::draw()
{
	VisualServer::Singleton()->update();
	m_worldRenderStates.transform = sf::Transform();
	sf::RenderWindow& window = *VisualServer::GetWindow();

	for (const auto& p : Engine::m_objsRegistery)
	{
		p.second->draw(window, m_worldRenderStates);
	}

	if (Engine::m_drawAction)
		Engine::m_drawAction(window, m_worldRenderStates);
}

void Engine::finalize_program()
{
	Engine* engine = Engine::s_instance;
	assert_msg(
		engine,
		"No R2D engine created and set to the 'mainEngine' global field, use r2d::Engine::Fire() to luanch the r2d engine"
	);
	assert_msg(
		engine->m_mainLoopLocked,
		"You should call 'start' on the main R2D engine signleton to start the program or the program will close immedialty"
	);
}

void Engine::main_loop()
{
	assert_msg(!m_mainLoopLocked, "already running main loop");
	m_mainLoopLocked = true;

	sf::RenderWindow* MainWindow = VisualServer::GetWindow();
	sf::Event window_event;
	if (Engine::m_onInitAction)
		Engine::m_onInitAction();
	while (MainWindow->isOpen())
	{
		std::chrono::steady_clock::time_point current_frame_time = std::chrono::high_resolution_clock::now();
		m_physicsDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(current_frame_time - m_lastFrameTime).count() / 1'000'000.0f;
		m_lastFrameTime = current_frame_time;
		m_currentFrame++;

		MainWindow->clear(R2D_CLEAR_CLR);
		// check all events
		while (MainWindow->pollEvent(window_event)) {

			switch (window_event.type)
			{
				case sf::Event::MouseMoved:
					m_lastMousePosition = m_mousePosition;
					m_mousePosition.x = (float)window_event.mouseMove.x;
					m_mousePosition.y = (float)window_event.mouseMove.y;
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
		update();
		physics();
		draw();

		MainWindow->display();
	}
}


bool Engine::isMouseButtonPressed(sf::Mouse::Button button) const
{
	return m_mouseInput.at(button).pressed;
}

bool Engine::isMouseButtonJustPressed(sf::Mouse::Button button) const
{
	return m_mouseInput.at(button).pressed && m_mouseInput.at(button).frame == m_currentFrame;
}

bool Engine::isMouseButtonJustReleased(sf::Mouse::Button button) const
{
	return !m_mouseInput.at(button).pressed && m_mouseInput.at(button).frame == m_currentFrame;
}

bool Engine::isScrollingUp() const
{
	return m_mouseWheelV > 0;
}

bool Engine::isScrollingDown() const
{
	return m_mouseWheelV < 0;
}

bool Engine::isScrollingLeft() const
{
	return m_mouseWheelV > 0;
}

bool Engine::isScrollingRight() const
{
	return m_mouseWheelV < 0;
}

bool Engine::isKeyPressed(sf::Keyboard::Key key) const
{
	return m_keyboardInput.at(key).pressed;
}

bool Engine::isKeyJustPressed(sf::Keyboard::Key key) const
{
	return m_keyboardInput.at(key).pressed && m_keyboardInput.at(key).frame == m_currentFrame;
}

bool Engine::isKeyJustReleased(sf::Keyboard::Key key) const
{
	return !m_keyboardInput.at(key).pressed && m_keyboardInput.at(key).frame == m_currentFrame;
}


void Engine::registerInput(const sf::Event::MouseButtonEvent& button_event, bool pressed)
{
	InputEvent& input_event = m_mouseInput[button_event.button];
	input_event.pressed = pressed;
	input_event.frame = m_currentFrame;
}

void Engine::registerInput(const sf::Event::MouseWheelScrollEvent& scroll_event)
{
	if (scroll_event.wheel == sf::Mouse::VerticalWheel)
		m_mouseWheelV = scroll_event.delta;
	else
		m_mouseWheelH = scroll_event.delta;
}

void Engine::registerInput(const sf::Event::KeyEvent& key_event, bool pressed)
{
	InputEvent& input_event = m_keyboardInput[key_event.code];
	input_event.pressed = pressed;
	input_event.frame = m_currentFrame;
}

_R2D_NAMESPACE_END_