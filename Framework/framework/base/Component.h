#pragma once

#include "../TinyDefinitions.h"

NS_TINY_BEGIN

class Object;

class Component
{
public:
	static const char* TRANSFORM;
	static const char* RENDERDER;

	Component();

	virtual bool init() = 0;
	virtual void update(float dt) { }
	virtual void release() { }

	virtual void setTarget(Object* target);

private:
	Object* _target;
};

NS_TINY_END