#include "CubeRenderer.h"
#include "..\base\Material.h"
#include "..\geometry\Cube.h"

USING_NS_TINY;

CubeRenderer::CubeRenderer()
{
}

CubeRenderer::~CubeRenderer()
{
	_texture->Release();
}

bool CubeRenderer::init()
{
	Renderer::init();

	_texture = nullptr;

	this->setMaterial(Material::WHITE_MATERIAL);

	return true;
}

void CubeRenderer::draw(LPDIRECT3DVERTEXBUFFER9 vertex, LPDIRECT3DINDEXBUFFER9 index)
{
	Renderer::draw();

	// select which vertex we're using
	_device->SetFVF(Cube::CUBE_FVF);

	// select vertex buffer to display
	_device->SetStreamSource(0, vertex, 0, sizeof(Cube::CubeVertex));
	_device->SetIndices(index);

	_device->SetMaterial(&_material);
	_device->SetTexture(0, _texture);
	
	// draw cube
	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
}

void CubeRenderer::setTexture(LPDIRECT3DTEXTURE9 texture)
{
	_texture = texture;
}

void CubeRenderer::setMaterial(const Material& material)
{
	_material.Ambient = Color4F(material.ambient.x, material.ambient.y, material.ambient.z, material.alpha);
	_material.Diffuse = Color4F(material.diffuse.x, material.diffuse.y, material.diffuse.z, material.alpha);
	_material.Specular = Color4F(material.specular.x, material.specular.y, material.specular.z, material.alpha);
	_material.Emissive = Color4F(material.emissive.x, material.emissive.y, material.emissive.z, material.alpha);
	_material.Power = material.shininess;
}
