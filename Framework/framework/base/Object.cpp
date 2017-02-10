#include "Object.h"
#include "Component.h"
#include "Transform.h"

USING_NS_TINY;

Object::Object() : 
	_transform(nullptr)
{
}

Object::~Object()
{
	for (auto it = _components.begin(); it != _components.end(); it++)
	{
		it->second->release();
		SAFE_DELETE(it->second);
	}

	_components.clear();
}

bool Object::init()
{
	_transform = new Transform();
	_transform->init();

	this->addComponent(Component::TRANSFORM, _transform);

	return true;
}

void Object::update(float dt)
{
	for (auto it = _components.begin(); it != _components.end(); it++)
	{
		it->second->update(dt);
	}
}

void Object::draw()
{
	
}

void Object::release()
{
}

void Object::addComponent(const char * name, Component * component)
{
	if (_components.find(name) != _components.end())
	{
		_components.at(name)->release();
	}

	_components[name] = component;
}

Component* Object::getComponent(const char * name)
{
	if (_components.find(name) != _components.end())
		return _components.at(name);

	return nullptr;
}

Transform * Object::getTransform()
{
	return _transform;
}