#include "TestScene.h"
#include "../Cube.h"

#include <functional>

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
	SAFE_DELETE(_testCube);
	SAFE_DELETE(_camera);
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

	_dispatcher = SceneManager::getInstance()->getEventDispatcher();

	//auto listener = new EventKeyboardListener();
	//listener->onKeyPressed = CALLBACK_01(&TestScene::onKeyPressed, this);
	//listener->onKeyReleased = CALLBACK_01(&TestScene::onKeyReleased, this);

	//_dispatcher->addEventListener(listener, this);

	return true;
}

void TestScene::update(float dt)
{
	_camera->update(dt);
}

void TestScene::draw()
{
	_testCube->draw();
}

void TestScene::onKeyPressed(EventKeyboard::KeyCode keycode)
{
	OutputDebugString("key pressed!!!\n");
}

void TestScene::onKeyReleased(EventKeyboard::KeyCode keycode)
{
	OutputDebugString("key released!!!\n");
}
