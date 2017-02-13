#pragma once

#include "..\TinyDefinitions.h"
#include "..\base\Renderer.h"

NS_TINY_BEGIN

class CubeRenderer : public tiny::Renderer
{
public:
	CubeRenderer();
	~CubeRenderer();

	virtual bool init() override;
	virtual void draw(LPDIRECT3DVERTEXBUFFER9 vertex, LPDIRECT3DINDEXBUFFER9 index, LPDIRECT3DTEXTURE9 texture);

private:
};

NS_TINY_END