#pragma once

#include <Windows.h>
#include "TinyDefinitions.h"

NS_TINY_BEGIN

class WindowsHelper
{
public:
	static WindowsHelper* create(HINSTANCE hInstance, const char* name, int width, int height);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	WindowsHelper();
	~WindowsHelper();

	bool init(HINSTANCE hInstance, const char* name, int width, int height);
	
	bool isOpen();
	
	void translateMessage();

	void setWindowSize(int width, int height);
	int getWindowWidth();
	int getWindowHeight();

	HWND getWindowHandle();
	HINSTANCE getWindowInstance();

private:
	HINSTANCE _hInstance;
	HWND _windowHandle;
	MSG _message;

	const char* _windowName;
	int _width;
	int _height;

	bool _isOpen;

};

NS_TINY_END