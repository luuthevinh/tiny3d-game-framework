#include "EventMouse.h"

USING_NS_TINY;

EventMouse::EventMouse(Button button)
{
	_button = button;
	_deltaChange.x = 0.0f;
	_deltaChange.y = 0.0f;
	_isPressed = false;
	_type = Type::MOUSE;
}

EventMouse::EventMouse(float x, float y)
{
	_button = Button::BUTTON_NONE;
	_deltaChange.x = x;
	_deltaChange.y = y;
	_isPressed = false;
	_type = Type::MOUSE;
}

EventMouse::~EventMouse()
{
}

const EventMouse::Button& EventMouse::getButton() const
{
	return _button;
}

void EventMouse::setButton(const Button & button)
{
	_button = button;
}

void EventMouse::setPressed(bool pressed)
{
	_isPressed = pressed;
}

bool EventMouse::isPressed()
{
	return _isPressed;
}

void EventMouse::setDeltaChange(float x, float y)
{
	_deltaChange.x = x;
	_deltaChange.y = y;
}

const Vec2 & EventMouse::getDeltaChange() const
{
	return _deltaChange;
}

void EventMouse::setPosition(const Vec2 & position)
{
	_position = position;
}

const Vec2 & EventMouse::getPosition() const
{
	return _position;
}
