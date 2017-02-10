#pragma once

#include <functional>
#include "EventListener.h"
#include "EventKeyboard.h"

NS_TINY_BEGIN

class EventKeyboardListener : public EventListener
{
public:
	EventKeyboardListener();
	~EventKeyboardListener();

	// Inherited via EventListener
	virtual void onNotify(Event * event) override;

	std::function<void(EventKeyboard::KeyCode)> onKeyPressed;
	std::function<void(EventKeyboard::KeyCode)> onKeyReleased;

private:

};

NS_TINY_END