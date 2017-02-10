#include "CubeRenderer.h"
#include "Cube.h"

CubeRenderer::CubeRenderer()
{
}

CubeRenderer::~CubeRenderer()
{
}

bool CubeRenderer::init()
{
	_device = SceneManager::getInstance()->getDevice()->getDirec3DDevice();

	return true;
}

void CubeRenderer::draw(const Transform & transform, LPDIRECT3DVERTEXBUFFER9 vertex, LPDIRECT3DINDEXBUFFER9 index, LPDIRECT3DTEXTURE9 texture)
{
	// select which vertex we're using
	_device->SetFVF(Cube::CUBE_FVF);

	// select vertex buffer to display
	_device->SetStreamSource(0, vertex, 0, sizeof(Cube::CubeVertex));
	_device->SetIndices(index);
	_device->SetTexture(0, texture);

	// transform
	auto position = transform.getPosition();
	auto scale = transform.getScale();
	auto rotate = transform.getRotate();

	// scale
	D3DXMATRIX scaleMat;
	D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);

	// rotate
	D3DXMATRIX rotateMatX;
	D3DXMATRIX rotateMatY;
	D3DXMATRIX rotateMatZ;
	D3DXMatrixRotationX(&rotateMatX, D3DXToRadian(rotate.x));
	D3DXMatrixRotationY(&rotateMatY, D3DXToRadian(rotate.y));
	D3DXMatrixRotationZ(&rotateMatZ, D3DXToRadian(rotate.z));

	// translate
	D3DXMATRIX translateMat;
	D3DXMatrixTranslation(&translateMat, position.x, position.y, position.z);

	// set transform
	_device->SetTransform(D3DTS_WORLD, &(rotateMatX * rotateMatY * rotateMatZ * scaleMat * translateMat));

	// draw cube
	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
}