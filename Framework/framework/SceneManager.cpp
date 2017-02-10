#include "SceneManager.h"

USING_NS_TINY;

SceneManager* SceneManager::_instance = nullptr;

SceneManager::~SceneManager()
{
}

SceneManager::SceneManager()
{
}

SceneManager * SceneManager::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new SceneManager();
	}

	return _instance;
}

void SceneManager::update(float dt)
{
	if (!_scenes.empty())
		_scenes.back()->update(dt);
}

void SceneManager::draw()
{
	if (!_scenes.empty())
		_scenes.back()->draw();
}

void SceneManager::release()
{
	this->clearScenes();

	delete _dispatcher;

	delete this;
}

void SceneManager::addScene(Scene * scene)
{
	_scenes.push_back(scene);
	_scenes.back()->init();
}

void SceneManager::removeScene()
{
	if (!_scenes.empty())
	{
		_scenes.back()->release();
		delete _scenes.back();

		_scenes.pop_back();
	}
}

void SceneManager::replaceScene(Scene * scene)
{
	this->removeScene();
	this->addScene(scene);
}

void SceneManager::clearScenes()
{
	while (!_scenes.empty())
	{
		_scenes.back()->release();
		delete _scenes.back();

		_scenes.pop_back();
	}
}

Scene * SceneManager::getCurrentScene()
{
	if (!_scenes.empty())
		return _scenes.back();

	return nullptr;
}

void SceneManager::setDevice(Direct3DDevice * device)
{
	_device = device;
}

Direct3DDevice * SceneManager::getDevice()
{
	return _device;
}

void SceneManager::setDispatcher(EventDispatcher * dispatcher)
{
	_dispatcher = dispatcher;
}

EventDispatcher * SceneManager::getEventDispatcher()
{
	return _dispatcher;
}

