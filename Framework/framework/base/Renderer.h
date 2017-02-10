#pragma once

#include "..\TinyDefinitions.h"
#include "Component.h"
#include "Transform.h"

NS_TINY_BEGIN

class Renderer : public Component
{
public:
	// Inherited via Component
	virtual bool init() override;

	virtual void draw(const Transform& transform);

private:
};

NS_TINY_END