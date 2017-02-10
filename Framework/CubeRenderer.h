#pragma once

#include "Tiny3D.h"

USING_NS_TINY;

class CubeRenderer : public tiny::Renderer
{
public:
	CubeRenderer();
	~CubeRenderer();

	virtual bool init() override;
	virtual void draw(const Transform& transform, LPDIRECT3DVERTEXBUFFER9 vertex, LPDIRECT3DINDEXBUFFER9 index, LPDIRECT3DTEXTURE9 texture);

private:
	LPDIRECT3DDEVICE9 _device;
};
