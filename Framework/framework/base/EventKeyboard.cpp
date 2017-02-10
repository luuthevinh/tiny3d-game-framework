#include "EventKeyboard.h"

USING_NS_TINY;

EventKeyboard::EventKeyboard(KeyCode keycode)
{
	_keyCode = keycode;
	_isPressed = false;
	_type = Event::Type::KEYBOARD;
}

EventKeyboard::~EventKeyboard()
{
}

const EventKeyboard::KeyCode& EventKeyboard::getKeycode() const
{
	return _keyCode;
}

bool EventKeyboard::isPressed() const
{
	return _isPressed;
}

void EventKeyboard::setPressed(bool pressed)
{
	_isPressed = pressed;
}