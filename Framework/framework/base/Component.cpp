#include "Component.h"
#include "Object.h"

NS_TINY_BEGIN

const char* Component::TRANSFORM = "transform";
const char* Component::RENDERDER = "renderer";

Component::Component()
{
	_target = nullptr;
}

void Component::setTarget(Object* target)
{
	_target = target;
}


NS_TINY_END
