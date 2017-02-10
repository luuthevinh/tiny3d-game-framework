#include "WindowsHelper.h"
#include "..\Settings.h"

USING_NS_TINY;

WindowsHelper * WindowsHelper::create(HINSTANCE hInstance, const char * name, int width, int height)
{
	auto window = new WindowsHelper();
	if (window->init(hInstance, name, width, height))
	{
		return window;
	}

	delete window;
	return nullptr;
}


WindowsHelper::WindowsHelper()
{
}

WindowsHelper::~WindowsHelper()
{
}

bool WindowsHelper::init(HINSTANCE hInstance, const char * name, int width, int height)
{
	_isOpen = true;
	_hInstance = hInstance;
	_windowName = name;
	_width = width;
	_height = height;

	auto settings = Settings::getInstance();

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowsHelper::WindowProc;
	wc.hInstance = _hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass";

	DWORD style;
	if (!settings->isFullScreen())
	{
		style = WS_OVERLAPPEDWINDOW;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	}
	else
	{
		style = WS_EX_TOPMOST | WS_POPUP;
	}

	RegisterClassEx(&wc);

	

	_windowHandle = CreateWindowEx(
		NULL,
		"WindowClass",
		_windowName,
		style,
		settings->getWindowStartPosition().x,
		settings->getWindowStartPosition().y,
		_width, 
		_height,
		NULL,
		NULL,
		_hInstance,
		NULL);

	ShowWindow(_windowHandle, SW_SHOWNORMAL);
	UpdateWindow(_windowHandle);

	return true;
}

bool WindowsHelper::isOpen()
{
	return _isOpen;
}

LRESULT CALLBACK WindowsHelper::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} 
		default:
			break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void WindowsHelper::translateMessage()
{
	while (PeekMessage(&_message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&_message);
		DispatchMessage(&_message);
	}

	if (_message.message == WM_QUIT)
	{
		_isOpen = false;
	}
}

void WindowsHelper::setWindowSize(int width, int height)
{
	_width = width;
	_height = height;
}

int WindowsHelper::getWindowWidth()
{
	return _width;
}

int WindowsHelper::getWindowHeight()
{
	return _height;
}

HWND WindowsHelper::getWindowHandle()
{
	return _windowHandle;
}

HINSTANCE WindowsHelper::getWindowInstance()
{
	return _hInstance;
}


