#include "EventKeyboardListener.h"
#include "EventKeyboard.h"

USING_NS_TINY;

EventKeyboardListener::EventKeyboardListener()
{
	
}

EventKeyboardListener::~EventKeyboardListener()
{
}

void EventKeyboardListener::onNotify(Event * event)
{
	if (event->getType() != Event::Type::KEYBOARD)
		return;

	auto keyboardEvent = (EventKeyboard*)(event);

	if (keyboardEvent->isPressed())
	{
		if (this->onKeyPressed != nullptr)
		{
			this->onKeyPressed(keyboardEvent->getKeycode());
		}
	}
	else
	{
		if (this->onKeyReleased != nullptr)
		{
			this->onKeyReleased(keyboardEvent->getKeycode());
		}
	}
}
