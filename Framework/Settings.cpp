#include "Settings.h"
#include "Definitions.h"
#include "Tiny3D.h"

#include "Scenes\TestScene.h"

NS_TINY_BEGIN

Settings* Settings::_instance = nullptr;

Settings * Settings::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new Settings();
		_instance->init();
	}

	return _instance;
}

Settings::~Settings()
{
}

bool Settings::init()
{
	this->setFullScreen(false);
	this->setWindowSize(Size(WINDOW_WIDHT, WINDOW_HEIGHT));

	this->enableLight(true);

	this->setFrameRate(1.0f / 60.0f);

	this->setWindowStartPosition(Vec2(100.0f, 100.0f));
	this->setMouseStartPosition(Vec2(WINDOW_WIDHT / 2, WINDOW_HEIGHT / 2));

	return true;
}

void Settings::release()
{
	delete this;
}

bool Settings::isFullScreen()
{
	return _isFullScreen;
}

void Settings::setFullScreen(bool value)
{
	_isFullScreen = value;
}

bool Settings::isEnabledLight()
{
	return _lightEnabled;
}

void Settings::enableLight(bool value)
{
	_lightEnabled = value;
}

const Size & Settings::getWindowSize()
{
	return _windowSize;
}

void Settings::setWindowSize(const Size & size)
{
	_windowSize = size;
}

float Settings::getFrameRate()
{
	return _frameRate;
}

void Settings::setFrameRate(float frameRate)
{
	_frameRate = frameRate;
}

const Vec2 & Settings::getMouseStartPosition()
{
	return _windowStartPosition + _mouseStartPosition;
}

void Settings::setMouseStartPosition(const Vec2 & position)
{
	_mouseStartPosition = position;
}

const Vec2 & Settings::getWindowStartPosition()
{
	return _windowStartPosition;
}

void Settings::setWindowStartPosition(const Vec2 & position)
{
	_windowStartPosition = position;
}

NS_TINY_END