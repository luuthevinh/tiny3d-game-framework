#include "TestScene.h"
#include "..\framework\geometry\Cube.h"

USING_NS_TINY;

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
	
}

bool TestScene::init()
{
	_testCube = new Cube();
	_testCube->init("cat.png");

	_camera = new Camera(
		Vec3(20.0f, 50.0f, -45.0f),
		Vec3(0.0f, 0.0f, 0.0f),
		Vec3(0.0f, 1.0f, 0.0f));
	_camera->init();

	_mesh = new Mesh();
	//_mesh->initWithXFile("airplane.x");
	_mesh->initWithOBJFile("resources\\cartoon_tank.obj");
	_mesh->getTransform()->setPositionX(20.0f);

	return true;
}

void TestScene::update(float dt)
{
	_camera->update(dt);
}

void TestScene::draw()
{
	_testCube->draw();
	_mesh->draw();
}

void TestScene::release()
{
	_camera->release();
	_testCube->release();
	_mesh->release();
}
