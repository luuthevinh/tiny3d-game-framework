#include "MeshRenderer.h"
#include "..\base\Object.h"

USING_NS_TINY;

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::draw()
{
	Renderer::draw();

	for (size_t i = 0; i < _materials.size(); i++)
	{
		_device->SetMaterial(&_materials[i]);
		_device->SetTexture(0, _textures[i]);

		_meshD3D->DrawSubset(i);

		// draw wireframe
		//_device->SetMaterial(&material);
		//_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//_meshD3D->DrawSubset(i);

		_device->SetRenderState(D3DRS_FILLMODE, this->getFillMode());
	}
}

void MeshRenderer::release()
{
	_meshD3D->Release();

	while (_materials.size() > 0)
	{
		_materials.pop_back();
	}

	while (_textures.size() > 0)
	{
		_textures.back()->Release();
		_textures.pop_back();
	}
}

void MeshRenderer::setD3DMesh(LPD3DXMESH mesh)
{
	_meshD3D = mesh;
}

void MeshRenderer::addMaterial(D3DMATERIAL9 material)
{
	_materials.push_back(material);
}

void MeshRenderer::addTexture(LPDIRECT3DTEXTURE9 texture)
{
	_textures.push_back(texture);
}
