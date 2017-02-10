#pragma once

#include "..\TinyDefinitions.h"

NS_TINY_BEGIN

class Event
{
public:
	enum Type
	{
		KEYBOARD,
		MOUSE
	};

	virtual void update(float dt) { };

	virtual inline const Type& getType() const 
	{
		return _type;
	}

protected:
	Type _type;
};

NS_TINY_END