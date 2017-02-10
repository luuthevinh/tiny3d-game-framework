#include "DirectInputDevice.h"
#include "WindowsHelper.h"
#include "SceneManager.h"
#include "base\EventDispatcher.h"
#include "base\EventKeyboard.h"
#include "base\EventMouse.h"

#include "..\Settings.h"

USING_NS_TINY;

DirectInputDevice::DirectInputDevice()
{
	_input = nullptr;
	_keyboard = nullptr;
	_mouse = nullptr;
}

DirectInputDevice * DirectInputDevice::create(WindowsHelper * window)
{
	auto input = new DirectInputDevice();
	if (input->init(window))
	{
		return input;
	}

	SAFE_DELETE(input);
	return nullptr;
}

DirectInputDevice::~DirectInputDevice()
{
}

bool DirectInputDevice::init(WindowsHelper * window)
{
	_window = window;
	auto hwnd = window->getWindowHandle();
	auto hInstance = window->getWindowInstance();

	_dispatcher = SceneManager::getInstance()->getEventDispatcher();

	_screenWidth = window->getWindowWidth();
	_screenHeight = window->getWindowHeight();

	_mouseX = Settings::getInstance()->getMouseStartPosition().x;
	_mouseY = Settings::getInstance()->getMouseStartPosition().y;

	HRESULT result;
	result = DirectInput8Create(
				hInstance,
				DIRECTINPUT_VERSION,
				IID_IDirectInput8,
				(void**)&_input,
				NULL);

	if (FAILED(result))
		return false;

	result = _input->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);
	if (FAILED(result))
		return false;

	result = _keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	result = _keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return false;

	DIPROPDWORD props;
	props.diph.dwSize = sizeof(DIPROPDWORD);
	props.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	props.diph.dwHow = DIPH_DEVICE;
	props.diph.dwObj = 0;
	props.dwData = VIRTUAL_KEY_NUMBER;

	result = _keyboard->SetProperty(DIPROP_BUFFERSIZE, &props.diph);
	if (FAILED(result))
		return false;

	result = _keyboard->Acquire();
	if (FAILED(result))
		return false;

	// mouse
	result = _input->CreateDevice(GUID_SysMouse, &_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = _mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	result = _mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = _mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void DirectInputDevice::update()
{
	// mouse
	_mouseState.lX = 0.0f;
	_mouseState.lY = 0.0f;

	HRESULT result = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&_mouseState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			_mouse->Acquire();
		}
	}


	if (_mouseState.lX != 0 || _mouseState.lY != 0)
	{
		_mouseX += _mouseState.lX;
		_mouseY += _mouseState.lY;

		auto mouseEvent = new EventMouse(_mouseState.lX, _mouseState.lY);
		mouseEvent->setPosition(getCursorPosition());

		if ((_mouseState.rgbButtons[0] & 0x80) > 0)
		{
			mouseEvent->setPressed(true);
			mouseEvent->setButton(EventMouse::Button::BUTTON_LEFT);
		}
		else if ((_mouseState.rgbButtons[1] & 0x80) > 0)
		{
			mouseEvent->setPressed(true);
			mouseEvent->setButton(EventMouse::Button::BUTTON_RIGHT);
		}

		_dispatcher->notify(mouseEvent);

		SAFE_DELETE(mouseEvent);
	}

	// key board
	ZeroMemory(_keyData, sizeof(_keyData));
	DWORD numKeys = VIRTUAL_KEY_NUMBER;
	result = _keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), _keyData, &numKeys, 0);
	
	if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
	{
		if (FAILED(_keyboard->Acquire()))
		{
			return;
		}
	}

	for (DWORD i = 0; i < numKeys; i++)
	{
		int keycode = _keyData[i].dwOfs;
		if (keycode == 0)
			continue;

		int keystate = _keyData[i].dwData;  

		auto keyboardEvent = new EventKeyboard((EventKeyboard::KeyCode)keycode);

		if ((keystate & 0x80) > 0)
		{
			keyboardEvent->setPressed(true);
			_dispatcher->notify(keyboardEvent);
		}
		else
		{
			keyboardEvent->setPressed(false);
			_dispatcher->notify(keyboardEvent);
		}

		SAFE_DELETE(keyboardEvent);
	}
}

void DirectInputDevice::release()
{
	if (_keyboard != nullptr)
	{
		_keyboard->Unacquire();
		_keyboard->Release();
	}
	
	if (_mouse != nullptr)
	{
		_mouse->Unacquire();
		_mouse->Release();
	}
	
	if (_input != nullptr)
	{
		_input->Release();
	}

	delete this;
}

const Vec2 & DirectInputDevice::getCursorPosition()
{
	POINT pos;
	GetCursorPos(&pos);
	RECT rect;

	GetWindowRect(_window->getWindowHandle(), &rect);
	pos.x -= rect.left;
	pos.y -= rect.top;

	return Vec2(pos.x, _window->getWindowHeight() - pos.y);
}


