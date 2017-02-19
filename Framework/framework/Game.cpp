#include "Game.h"
#include "Direct3DDevice.h"
#include "DirectInputDevice.h"
#include "WindowsHelper.h"
#include "base\EventDispatcher.h"

#include "..\Settings.h"
#include "Camera.h"
#include "Utils.h"
#include "SceneManager.h"
#include "base\Time.h"

#include <ctime>
#include <stdio.h>

USING_NS_TINY;

Game::Game(HINSTANCE hInstance, const char* name, int width, int height)
{
	_gameSettings = Settings::getInstance();
	_gameSettings->setWindowSize(Size(width, height));

	_window = WindowsHelper::create(hInstance, name, width, height);
}

Game::~Game()
{
}

bool Game::init()
{
	// direct3d
	_direct3d = Direct3DDevice::createWithWindow(_window);
	SceneManager::getInstance()->setDevice(_direct3d);

	// event dispatcher
	auto dispatcher = new EventDispatcher();
	SceneManager::getInstance()->setDispatcher(dispatcher);

	// input
	_input = DirectInputDevice::create(_window);

	// time
	_time = new Time();
	_time->init();

	// test
	_direct3d->getDirec3DDevice()->SetMaterial(&Utils::WHITE_MATERIAL);

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = Color4F(1.0f, 1.0f, 1.0f, 1.0f);
	light.Direction = Vec3(-1.0f, -1.5f, 2.0f);

	_direct3d->getDirec3DDevice()->SetLight(0, &light);
	_direct3d->getDirec3DDevice()->LightEnable(0, TRUE);
	// end test

	return true;
}

void Game::run()
{
	float lag = 0.0f;

	while (_window->isOpen())
	{
		// translate
		_window->translateMessage();

		// game time
		_time->update();

		// input
		_input->update();

		float delta = _time->getTotalTime() - _lastTime;
		
		if (delta >= _gameSettings->getFrameRate())
		{
			//_lastTime += delta;

			_lastTime = _time->getTotalTime();
			lag += delta;

			char str[128];
			sprintf(str, "delta: %.4f | total: %.2f | lag: %.4f | Tiny3D Game Framework", delta, _time->getTotalTime(), lag);
			SetWindowText(_window->getWindowHandle(), str);

			while (lag >= _gameSettings->getFrameRate())
			{
				// update
				SceneManager::getInstance()->update(_gameSettings->getFrameRate());
				lag -= _gameSettings->getFrameRate();
			}
			
			// draw
			this->draw();
		}
		//else
		//{
			//Sleep((_gameSettings->getFrameRate() - delta) * 1000.0f);
		//}
	}
}

void Game::release()
{
	delete _window;
	delete _time;

	_direct3d->release();
	_input->release();

	Settings::getInstance()->release();
	SceneManager::getInstance()->release();

	delete this;
}

bool Game::isRunning()
{
	return true;
}

void Game::draw()
{
	_direct3d->getDirec3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET, Color3B(100, 100, 100), 1.0f, 0);
	_direct3d->getDirec3DDevice()->Clear(0, NULL, D3DCLEAR_ZBUFFER, Color3B(0, 0, 0), 1.0f, 0);

	_direct3d->getDirec3DDevice()->BeginScene();

	// draw
	SceneManager::getInstance()->draw();

	_direct3d->getDirec3DDevice()->EndScene();

	_direct3d->getDirec3DDevice()->Present(NULL, NULL, NULL, NULL);
}


