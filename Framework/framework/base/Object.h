#pragma once

#include <map>
#include <string>

#include "..\TinyDefinitions.h"

NS_TINY_BEGIN

class Component;
class Transform;

class Object
{
public:
	Object();
	~Object();

	virtual bool init();
	virtual void update(float dt);
	virtual void draw();
	virtual void release();

	virtual void addComponent(const char* name, Component* component);
	virtual Component* getComponent(const char* name);

	Transform* getTransform();

protected:
	std::map<std::string, Component*> _components;

	Transform* _transform;
};

NS_TINY_END