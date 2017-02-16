#pragma once

#include "..\Tiny3D.h"

USING_NS_TINY;

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	virtual bool init() override;
	virtual void update(float dt) override;
	virtual void draw() override;
	virtual void release() override;

private:
	Cube* _testCube;
	Camera* _camera;
	Mesh* _mesh;
};
