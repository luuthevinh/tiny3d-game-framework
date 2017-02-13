#include "CubeRenderer.h"

#include "..\geometry\Cube.h"

USING_NS_TINY;

CubeRenderer::CubeRenderer()
{
}

CubeRenderer::~CubeRenderer()
{
}

bool CubeRenderer::init()
{
	Renderer::init();

	return true;
}

void CubeRenderer::draw(LPDIRECT3DVERTEXBUFFER9 vertex, LPDIRECT3DINDEXBUFFER9 index, LPDIRECT3DTEXTURE9 texture)
{
	Renderer::draw();

	// select which vertex we're using
	_device->SetFVF(Cube::CUBE_FVF);

	// select vertex buffer to display
	_device->SetStreamSource(0, vertex, 0, sizeof(Cube::CubeVertex));
	_device->SetIndices(index);
	_device->SetTexture(0, texture);

	// draw cube
	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
}