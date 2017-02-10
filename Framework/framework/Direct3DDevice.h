#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "TinyDefinitions.h"

NS_TINY_BEGIN

class WindowsHelper;

class Direct3DDevice
{
public:
	static Direct3DDevice* createWithWindow(WindowsHelper* window);

	Direct3DDevice();
	~Direct3DDevice();

	bool init(WindowsHelper* window);
	void draw();
	void release();

	LPDIRECT3DDEVICE9 getDirec3DDevice();

private:
	HWND _hWnd;
	LPDIRECT3D9 _d3d;
	LPDIRECT3DDEVICE9 _d3dDevice;
};

NS_TINY_END