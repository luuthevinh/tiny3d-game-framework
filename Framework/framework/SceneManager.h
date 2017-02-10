#pragma once

#include <vector>
#include "base\Scene.h"
#include "TinyDefinitions.h"
#include "Direct3DDevice.h"

NS_TINY_BEGIN

class EventDispatcher;

class SceneManager
{
public:
	static SceneManager* getInstance();
	~SceneManager();

	void addScene(Scene* scene);
	void removeScene();
	void replaceScene(Scene* scene);
	void clearScenes();

	void update(float dt);
	void draw();
	void release();

	Scene* getCurrentScene();

	void setDevice(Direct3DDevice* device);
	Direct3DDevice* getDevice();

	void setDispatcher(EventDispatcher* dispatcher);
	EventDispatcher* getEventDispatcher();

private:
	SceneManager();
	static SceneManager* _instance;

	std::vector<Scene*> _scenes;
	Direct3DDevice* _device;
	EventDispatcher* _dispatcher;
};

NS_TINY_END
