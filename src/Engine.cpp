#include "global.h"
#include "Engine.h"


_R2D_NAMESPACE_START_

ErrorCode Engine::Init()
{
	atexit(Engine::Final);
	return ErrorCode::Ok;
}

ErrorCode Engine::Fire()
{
	VisualServer::Start();
	Engine::Main();

	return ErrorCode::Ok;
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

uint32_t Engine::GetCurrentFrame()
{
	return s_currentFrame;
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
				Input::s_lastMousePosition = Input::s_mousePosition;
				Input::s_mousePosition.x = (float)window_event.mouseMove.x;
				Input::s_mousePosition.y = (float)window_event.mouseMove.y;
				Input::s_worldMousePosition = Input::s_mousePosition + VisualServer::GetScreenOrigin();
				break;
			case sf::Event::KeyPressed:
				Input::registerInput(window_event.key, true);
				break;
			case sf::Event::KeyReleased:
				Input::registerInput(window_event.key, false);
				break;
			case sf::Event::MouseButtonPressed:
				Input::registerInput(window_event.mouseButton, true);
				break;
			case sf::Event::MouseButtonReleased:
				Input::registerInput(window_event.mouseButton, false);
				break;
			case sf::Event::MouseWheelScrolled:
				Input::registerInput(window_event.mouseWheelScroll);
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



_R2D_NAMESPACE_END_