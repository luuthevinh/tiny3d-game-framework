#include "EventDispatcher.h"
#include "Event.h"

USING_NS_TINY;

EventDispatcher::EventDispatcher()
{
	_numberListeners = 0;
}

EventDispatcher::~EventDispatcher()
{
	for (auto it = _listeners.begin(); it != _listeners.end(); it++)
	{
		for (auto itListener = it->second.begin(); itListener != it->second.end(); itListener++)
		{
			SAFE_DELETE(*itListener);
		}

		it->second.clear();
	}

	_listeners.clear();
}

void EventDispatcher::addEventListener(EventListener * listener, Object* target)
{
	if (_listeners.find(target) != _listeners.end())
	{
		auto listeners = _listeners.at(target);
		
		auto result = std::find(listeners.begin(), listeners.end(), listener);
		if (result != listeners.end())
		{
			SAFE_DELETE(*result);
			listeners.erase(result);
		}
	}

	_listeners[target].push_back(listener);
}

void EventDispatcher::removeEventListener(EventListener * listener, Object* target)
{
	if (_listeners.find(target) != _listeners.end())
	{
		auto removeLis = std::find(_listeners.at(target).begin(), _listeners.at(target).end(), listener);
		if (removeLis != _listeners.at(target).end())
		{
			SAFE_DELETE(*removeLis);
			_listeners.at(target).erase(removeLis);
		}

		if (_listeners.at(target).size() == 0)
		{
			_listeners.erase(target);
		}
	}
}

void EventDispatcher::removeAllEventListener(Object * target)
{
	if (_listeners.find(target) != _listeners.end())
	{
		for (auto it = _listeners.at(target).begin(); it != _listeners.at(target).end(); it++)
		{
			SAFE_DELETE(*it);
		}

		_listeners.erase(target);
	}
}

void EventDispatcher::notify(Event* event)
{
	for (auto it = _listeners.begin(); it != _listeners.end(); it++)
	{
		for (auto itListener = it->second.begin(); itListener != it->second.end(); itListener++)
		{
			(*itListener)->onNotify(event);
		}
	}
}