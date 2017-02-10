#include "EventMouseListener.h"
#include "EventMouse.h"

USING_NS_TINY;

EventMouseListener::EventMouseListener()
{
}

EventMouseListener::~EventMouseListener()
{
}

void EventMouseListener::onNotify(Event * event)
{
	if (event->getType() != Event::Type::MOUSE)
	{
		return;
	}

	auto mouseEvent = (EventMouse*)event;

	if (mouseEvent->getDeltaChange() != Vec2(0.0f, 0.0f))
	{
		if (this->onMouseMoved != nullptr)
		{
			this->onMouseMoved(mouseEvent);
		}
		
	}

	if (mouseEvent->isPressed())
	{
		if (this->onMousePressed != nullptr)
		{
			this->onMousePressed(mouseEvent->getButton());
		}
		
	}
}
