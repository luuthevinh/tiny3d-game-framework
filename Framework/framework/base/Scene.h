#pragma once

#include "..\TinyDefinitions.h"
#include "Object.h"

NS_TINY_BEGIN

class Scene : public Object
{
public:
	virtual bool init() = 0;
	virtual void update(float dt) override { }
	virtual void draw() override { }
	virtual void release() { }

private:
};

NS_TINY_END