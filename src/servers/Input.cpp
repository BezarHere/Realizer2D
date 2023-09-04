#include "global.h"
#include "Input.h"
#include "../Engine.h"


_R2D_NAMESPACE_START_

ErrorCode Input::Init()
{

	for (int i = 0; i < Keyboard_t::KeyCount; i++)
	{
		s_keyboardInput[(Keyboard_t::Key)i];
	}

	for (int i = 0; i < Mouse_t::ButtonCount; i++)
	{
		s_mouseInput[(Mouse_t::Button)i];
	}

	return ErrorCode::Ok;
}

bool Input::IsMouseButtonPressed(sf::Mouse::Button button)
{
	return s_mouseInput.at(button).pressed;
}

bool Input::IsMouseButtonJustPressed(sf::Mouse::Button button)
{
	return s_mouseInput.at(button).pressed && s_mouseInput.at(button).frame == Engine::GetCurrentFrame();
}

bool Input::IsMouseButtonJustReleased(sf::Mouse::Button button)
{
	return !s_mouseInput.at(button).pressed && s_mouseInput.at(button).frame == Engine::GetCurrentFrame();
}

bool Input::IsScrollingUp()
{
	return s_mouseWheelV > 0;
}

bool Input::IsScrollingDown()
{
	return s_mouseWheelV < 0;
}

bool Input::IsScrollingLeft()
{
	return s_mouseWheelV > 0;
}

bool Input::IsScrollingRight()
{
	return s_mouseWheelV < 0;
}

bool Input::IsKeyPressed(sf::Keyboard::Key key)
{
	return s_keyboardInput.at(key).pressed;
}

bool Input::IsKeyJustPressed(sf::Keyboard::Key key)
{
	return s_keyboardInput.at(key).pressed && s_keyboardInput.at(key).frame == Engine::GetCurrentFrame();
}

bool Input::IsKeyJustReleased(sf::Keyboard::Key key)
{
	return !s_keyboardInput.at(key).pressed && s_keyboardInput.at(key).frame == Engine::GetCurrentFrame();
}

Vector2 Input::GetMousePosition()
{
	return s_worldMousePosition;
}

Vector2 Input::GetMouseScreenPosition()
{
	return s_mousePosition;
}


void Input::registerInput(const sf::Event::MouseButtonEvent& button_event, bool pressed)
{
	InputEvent& input_event = s_mouseInput[button_event.button];
	input_event.pressed = pressed;
	input_event.frame = Engine::GetCurrentFrame();
}

void Input::registerInput(const sf::Event::MouseWheelScrollEvent& scroll_event)
{
	if (scroll_event.wheel == sf::Mouse::VerticalWheel)
		s_mouseWheelV = scroll_event.delta;
	else
		s_mouseWheelH = scroll_event.delta;
}

void Input::registerInput(const sf::Event::KeyEvent& key_event, bool pressed)
{
	InputEvent& input_event = s_keyboardInput.at(key_event.code);
	input_event.pressed = pressed;
	input_event.frame = Engine::GetCurrentFrame();
}

_R2D_NAMESPACE_END_
