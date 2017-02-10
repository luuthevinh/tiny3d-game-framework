#include "Direct3DDevice.h"
#include "WindowsHelper.h"
#include "..\Settings.h"

USING_NS_TINY;

Direct3DDevice* Direct3DDevice::createWithWindow(WindowsHelper * window)
{
	auto directx = new Direct3DDevice();
	if (directx->init(window))
	{
		return directx;
	}

	delete directx;
	return nullptr;
}

Direct3DDevice::Direct3DDevice()
{
}

Direct3DDevice::~Direct3DDevice()
{
}

bool Direct3DDevice::init(WindowsHelper* window)
{
	_hWnd = window->getWindowHandle();

	// create d3d
	_d3d = Direct3DCreate9(D3D_SDK_VERSION);

	// Check for hardware vertexProcessing.
	D3DCAPS9 caps;
	_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	int vertexProcessing = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	auto settings = Settings::getInstance();

	// fullscreen
	if (settings->isFullScreen())
	{
		d3dpp.Windowed = FALSE;

		D3DDISPLAYMODE displayMode;
		_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

		d3dpp.BackBufferHeight = displayMode.Height;
		d3dpp.BackBufferWidth = displayMode.Width;

		settings->setWindowSize(Size(displayMode.Width, displayMode.Height));
		settings->setMouseStartPosition(Vec2(displayMode.Width / 2, displayMode.Height / 2));
		window->setWindowSize(displayMode.Width, displayMode.Height);
	}
	else
	{
		d3dpp.Windowed = TRUE;
		d3dpp.BackBufferHeight = window->getWindowHeight();
		d3dpp.BackBufferWidth = window->getWindowWidth();
	}

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = _hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// create device
	HRESULT result = _d3d->CreateDevice(
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		_hWnd, 
		vertexProcessing,
		&d3dpp, 
		&_d3dDevice);

	if (FAILED(result))
	{
		return false;
	}

	// set cursor
	SetCursorPos(settings->getMouseStartPosition().x, settings->getMouseStartPosition().y);

	// light
	if (settings->isEnabledLight())
	{
		_d3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	else
	{
		_d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
	
	_d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	_d3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));
	_d3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	_d3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	return true;
}

void Direct3DDevice::draw()
{
	if (_d3dDevice == nullptr)
		return;

	_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 200, 200), 1.0f, 0);
	_d3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 200, 200), 1.0f, 0);

	_d3dDevice->BeginScene();

	// draw

	_d3dDevice->EndScene();

	_d3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Direct3DDevice::release()
{
	_d3d->Release();
	_d3dDevice->Release();
}

LPDIRECT3DDEVICE9 Direct3DDevice::getDirec3DDevice()
{
	return _d3dDevice;
}

