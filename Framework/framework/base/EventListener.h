#pragma once

#include "..\TinyDefinitions.h"
#include "Event.h"

NS_TINY_BEGIN

class EventListener
{
public:
	virtual void onNotify(Event* event) = 0;

protected:
	Event::Type _type;
};

NS_TINY_END