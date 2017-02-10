#pragma once

#include <map>
#include <vector>
#include "..\TinyDefinitions.h"
#include "EventListener.h"

NS_TINY_BEGIN

class Event;
class Object;

class EventDispatcher
{
public:
	EventDispatcher();
	~EventDispatcher();

	void addEventListener(EventListener* listener, Object* target);
	void removeEventListener(EventListener* listener, Object* target);
	void removeAllEventListener(Object* target);

	void notify(Event* event);

private:
	std::map<Object*, std::vector<EventListener*>> _listeners;

	int _numberListeners;
};

NS_TINY_END