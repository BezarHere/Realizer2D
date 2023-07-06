#pragma once
#include "Object2D.h"
#include "components/Drawers.h"
#include "VisualServer.h"
#include "PhysicsServer.h"
#include "random.h"
#include "SceneTree.h"



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
	Engine();

	__forceinline static Engine* Singleton() { return Engine::s_instance; }

	// call to start the Realizer 2D's processes
	static void Fire();

	static void SetProcessAction(ProcessFunction_t action);
	static void SetPhysicsProcessAction(PhysicsFunction_t action);
	static void SetOnInitAction(Action_t action);

	static inline ProcessFunction_t GetProcessAction() { return m_processAction; }
	static inline PhysicsFunction_t GetPhysicsProcessAction() { return m_physicsAction; }
	static inline Action_t GetOnInitAction() { return m_onInitAction; }

	void RegisterObject(Object2D* object);
	void PopObject(Object2D* object);

	void start();

	bool isMouseButtonPressed(sf::Mouse::Button) const;
	bool isMouseButtonJustPressed(sf::Mouse::Button) const;
	bool isMouseButtonJustReleased(sf::Mouse::Button) const;

	bool isScrollingUp() const;
	bool isScrollingDown() const;
	bool isScrollingLeft() const;
	bool isScrollingRight() const;

	bool isKeyPressed(Keyboard_t::Key key) const;
	bool isKeyJustPressed(Keyboard_t::Key key) const;
	bool isKeyJustReleased(Keyboard_t::Key key) const;

private:
	struct InputEvent
	{
		bool pressed = false;
		uint32_t frame = 0;
		uint8_t flags = 0;
	};

	typedef float MouseWheelDelta_t;

	void main_loop();
	static void finalize_program();

	void update();
	void physics();
	void draw();

	void registerInput(const sf::Event::MouseButtonEvent& button_event, bool pressed);
	void registerInput(const sf::Event::MouseWheelScrollEvent& scroll_event);
	void registerInput(const sf::Event::KeyEvent& key_event, bool pressed);

private:
	static inline Engine* s_instance{nullptr};
	static inline ObjID_t m_objCounter = 0;
	static inline std::unordered_map<ObjID_t, Object2D*> m_objsRegistery{};

	static inline ProcessFunction_t m_processAction;
	static inline PhysicsFunction_t m_physicsAction;
	static inline Action_t m_onInitAction;

	uint32_t m_currentFrame;

	bool m_mainLoopLocked = false;

	sf::Vector2f m_mousePosition;
	sf::Vector2f m_lastMousePosition;

	std::unordered_map<Keyboard_t::Key, InputEvent> m_keyboardInput{};
	std::unordered_map<sf::Mouse::Button, InputEvent> m_mouseInput{};
	MouseWheelDelta_t m_mouseWheelV;
	MouseWheelDelta_t m_mouseWheelH;

	std::chrono::steady_clock::time_point m_lastFrameTime = std::chrono::high_resolution_clock::now();
	real_t m_physicsDeltaTime = 1.0f / 60.0f;

};

_R2D_NAMESPACE_END_