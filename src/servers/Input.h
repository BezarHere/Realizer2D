#pragma once
#include "../global.h"
#include "../core/Errors.h"

_R2D_NAMESPACE_START_

typedef sf::Keyboard Keyboard_t;
typedef sf::Mouse Mouse_t;
typedef sf::Vector2i CellIndex_t;
typedef float MouseWheelDelta_t;



class Input final
{
public:
	friend class Engine;
	friend class R2DInit;
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

	static void registerInput(const sf::Event::MouseButtonEvent& button_event, bool pressed);
	static void registerInput(const sf::Event::MouseWheelScrollEvent& scroll_event);
	static void registerInput(const sf::Event::KeyEvent& key_event, bool pressed);

private:
	Input() = delete;

	struct InputEvent
	{
		bool pressed = false;
		uint32_t frame = 0;
		uint8_t flags = 0;
	};

	static ErrorCode Init();

private:
	static inline sf::Vector2f s_mousePosition{ 0.0f, 0.0f };
	static inline sf::Vector2f s_worldMousePosition{ 0.0f, 0.0f };
	static inline sf::Vector2f s_lastMousePosition{ 0.0f, 0.0f };

	static inline std::unordered_map<Keyboard_t::Key, InputEvent> s_keyboardInput{};
	static inline std::unordered_map<sf::Mouse::Button, InputEvent> s_mouseInput{};
	static inline MouseWheelDelta_t s_mouseWheelV{ 0.0f };
	static inline MouseWheelDelta_t s_mouseWheelH{ 0.0f };
};

_R2D_NAMESPACE_END_