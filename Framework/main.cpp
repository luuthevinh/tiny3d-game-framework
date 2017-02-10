#include <Windows.h>

#include "Definitions.h"
#include "Framework\Game.h"
#include "Framework\SceneManager.h"
#include "Scenes\TestScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	auto game = new tiny::Game(hInstance, "Hello Tiny Framework!", WINDOW_WIDHT, WINDOW_HEIGHT);
	game->init();

	tiny::SceneManager::getInstance()->addScene(new TestScene());

	game->run();
	game->release();

	return 0;
}