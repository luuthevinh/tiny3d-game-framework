#pragma once

#include <functional>
#include "EventListener.h"
#include "EventMouse.h"

NS_TINY_BEGIN


class EventMouseListener : public EventListener
{
public:
	EventMouseListener();
	~EventMouseListener();

	// Inherited via EventListener
	virtual void onNotify(Event * event) override;

	std::function<void(EventMouse* e)> onMouseMoved;
	std::function<void(const EventMouse::Button& button)> onMousePressed;
private:
	


};

NS_TINY_END
