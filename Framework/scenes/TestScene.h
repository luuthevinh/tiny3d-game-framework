#pragma once

#include "..\Tiny3D.h"

class Cube;

USING_NS_TINY;

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	virtual bool init() override;
	virtual void update(float dt) override;
	virtual void draw() override;

private:
	Cube* _testCube;
	Camera* _camera;
};
