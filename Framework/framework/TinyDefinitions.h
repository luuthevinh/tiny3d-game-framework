#pragma once

#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>		// dinput8.lib, dxguid.lib

#define NS_TINY_BEGIN namespace tiny {
#define NS_TINY_END }

#define USING_NS_TINY using namespace tiny

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) \
if(p) \
{\
	delete (p); \
	p = nullptr; \
}
#endif // !SAFE_DELETE

typedef D3DXVECTOR3 Vec3;
typedef D3DXVECTOR2 Vec2;
typedef D3DXCOLOR Color4F;

#define Color3B(r, g, b) D3DCOLOR_XRGB(r, g, b)

#define CALLBACK_01(function, target) std::bind(function, target, std::placeholders::_1)