#pragma once
#include "scene/Object.h"
#include "components/Graphical.h"
#include "servers/VisualServer.h"
#include "servers/PhysicsServer.h"
#include "utils/Random.h"
#include "scene/SceneTree.h"



#ifndef R2D_TITLE
// funy
#define R2D_TITLE "Dude, define 'R2D_TITLE' before the r2d::Engine for a window title!"
#endif // !R2D_TITLE

#ifndef R2D_CLEAR_CLR
#define R2D_CLEAR_CLR sf::Color(12U, 12U, 12U, 0U)
#endif // !R2D_CLEAR_CLR

_R2D_NAMESPACE_START_
typedef sf::Keyboard Keyboard_t;
typedef sf::Mouse Mouse_t;
typedef sf::Vector2i CellIndex_t;


class Engine final
{
public:
	enum InputModifierFlags : uint8_t
	{
		None,
		LShift = 0x01,
		RShift = 0x02,
		Shift = LShift | RShift,
		LAlt = 0x04,
		RAlt = 0x08,
		Alt = LAlt | RAlt,
		LControl = 0x10,
		RControl = 0x20,
		Control = LControl | RControl,
	};

public:
	friend class VisualServer;
	friend class PhysicsServer;
	friend class Object2D;

	// call to start the Realizer 2D's processes
	[[nodiscard]] static Error Fire();

	static void SetProcessAction(ProcessFunction_t action);
	static void SetPhysicsProcessAction(PhysicsFunction_t action);
	static void SetOnInitAction(Action_t action);

	static inline ProcessFunction_t GetProcessAction() { return s_processAction; }
	static inline PhysicsFunction_t GetPhysicsProcessAction() { return s_physicsAction; }
	static inline Action_t GetOnInitAction() { return s_onInitAction; }

	static void RegisterObject(Object2D* object);
	static void PopObject(Object2D* object);

	static bool IsMouseButtonPressed(sf::Mouse::Button);
	static bool IsMouseButtonJustPressed(sf::Mouse::Button);
	static bool IsMouseButtonJustReleased(sf::Mouse::Button);

	static bool IsScrollingUp();
	static bool IsScrollingDown();
	static bool IsScrollingLeft();
	static bool IsScrollingRight();

	static bool IsKeyPressed(Keyboard_t::Key key);
	static bool IsKeyJustPressed(Keyboard_t::Key key);
	static bool IsKeyJustReleased(Keyboard_t::Key key);

	// the mouse position in global coords
	static Vector2 GetMousePosition();

	// the mouse in screen space:
	// (0, 0) is the mouse at the topleft of the screen,
	// (screen_size.x, screen_size.y) is the mouse at the bottom right.
	static Vector2 GetMouseScreenPosition();
private:
	Engine() = delete;
	Engine(const Engine& other) = delete;
	//inline Engine* operator new() = delete;
	static void Init();

	struct InputEvent
	{
		bool pressed = false;
		uint32_t frame = 0;
		uint8_t flags = 0;
	};

	typedef float MouseWheelDelta_t;

	static inline void Main();
	static void Final();

	static inline void Update();
	static inline void physics();
	static inline void draw();

	static inline void pollEvents();

	static inline void registerInput(const sf::Event::MouseButtonEvent& button_event, bool pressed);
	static inline void registerInput(const sf::Event::MouseWheelScrollEvent& scroll_event);
	static inline void registerInput(const sf::Event::KeyEvent& key_event, bool pressed);

private:
	// TODO: remove this
	static inline ObjID_t s_objCounter{ 0U };
	static inline std::unordered_map<ObjID_t, Object2D*> s_objsRegistery{};

	static inline ProcessFunction_t s_processAction{ nullptr };
	static inline PhysicsFunction_t s_physicsAction{ nullptr };
	static inline Action_t s_onInitAction{ nullptr };

	static inline uint32_t s_currentFrame{ 0U };

	static inline bool s_mainLoopLocked{ false };

	static inline sf::Vector2f s_mousePosition{ 0.0f, 0.0f };
	static inline sf::Vector2f s_worldMousePosition{ 0.0f, 0.0f };
	static inline sf::Vector2f s_lastMousePosition{ 0.0f, 0.0f };

	static inline std::unordered_map<Keyboard_t::Key, InputEvent> s_keyboardInput{};
	static inline std::unordered_map<sf::Mouse::Button, InputEvent> s_mouseInput{};
	static inline MouseWheelDelta_t s_mouseWheelV{ 0.0f };
	static inline MouseWheelDelta_t s_mouseWheelH{ 0.0f };

	static inline std::chrono::steady_clock::time_point s_lastFrameTime{ std::chrono::high_resolution_clock::now() };
	static inline real_t s_physicsDeltaTime{ 1.0f / 60.0f };

	static inline bool s_created{ false };
};

_R2D_NAMESPACE_END_