#pragma once

#include <Windows.h>
#include "TinyDefinitions.h"

class Cube;

NS_TINY_BEGIN

class WindowsHelper;
class Direct3DDevice;
class DirectInputDevice;
class Settings;
class Time;

class Game
{
public:
	Game(HINSTANCE hInstance, const char* name, int width, int height);
	~Game();

	bool init();
	void run();
	void release();

	bool isRunning();

private:
	WindowsHelper* _window;
	Direct3DDevice* _direct3d;
	DirectInputDevice* _input;

	Settings* _gameSettings;

	Time* _time;
	float _lastTime;

	void draw();
};

NS_TINY_END