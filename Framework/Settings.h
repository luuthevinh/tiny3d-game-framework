#pragma once

#include "framework\base\Size.h"
#include "framework\TinyDefinitions.h"

NS_TINY_BEGIN

class Settings
{
public:
	static Settings* getInstance();

	~Settings();

	bool init();
	void release();

	bool isFullScreen();
	void setFullScreen(bool value);

	bool isEnabledLight();
	void enableLight(bool value);

	const Size& getWindowSize();
	void setWindowSize(const Size& size);

	float getFrameRate();
	void setFrameRate(float frameRate);

	const Vec2& getMouseStartPosition();
	void setMouseStartPosition(const Vec2& position);

	const Vec2& getWindowStartPosition();
	void setWindowStartPosition(const Vec2& position);

private:
	static Settings* _instance;

	bool _isFullScreen;
	bool _lightEnabled;
	Size _windowSize;

	float _frameRate;

	Vec2 _mouseStartPosition;
	Vec2 _windowStartPosition;
};

NS_TINY_END