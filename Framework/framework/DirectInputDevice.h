#pragma once

#include "TinyDefinitions.h"

NS_TINY_BEGIN

class WindowsHelper;
class EventDispatcher;

#define VIRTUAL_KEY_NUMBER 256

class DirectInputDevice
{
public:
	static DirectInputDevice* create(WindowsHelper* window);

	DirectInputDevice();
	~DirectInputDevice();

	bool init(WindowsHelper* window);
	void update();
	void release();
private:
	WindowsHelper* _window;
	EventDispatcher* _dispatcher;
	LPDIRECTINPUT8 _input;

	LPDIRECTINPUTDEVICE8 _keyboard;
	DIDEVICEOBJECTDATA _keyData[VIRTUAL_KEY_NUMBER];

	LPDIRECTINPUTDEVICE8 _mouse;
	DIMOUSESTATE _mouseState;

	int _screenWidth, _screenHeight;
	int _mouseX, _mouseY;

	const Vec2& getCursorPosition();
};

NS_TINY_END