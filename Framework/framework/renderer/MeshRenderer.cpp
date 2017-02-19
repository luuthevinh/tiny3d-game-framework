#include "MeshRenderer.h"
#include "..\base\Object.h"
#include "..\base\Material.h"

#include "..\Utils.h"

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

		_device->SetRenderState(D3DRS_FILLMODE, this->getFillMode());
	}
}

void MeshRenderer::release()
{
	if (_meshD3D != nullptr)
	{
		_meshD3D->Release();
	}
	
	while (_materials.size() > 0)
	{
		_materials.pop_back();
	}

	while (!_textures.empty())
	{
		if (_textures.back() != nullptr)
		{
			_textures.back()->Release();
		}
		
		_textures.pop_back();
	}

	delete this;
}

void MeshRenderer::setD3DMesh(LPD3DXMESH mesh)
{
	_meshD3D = mesh;
}

void MeshRenderer::addD3DMaterial(const D3DMATERIAL9& material)
{
	_materials.push_back(material);
}

void MeshRenderer::addMaterial(const Material & material)
{
	D3DMATERIAL9 d3dMaterial;

	d3dMaterial.Ambient = Color4F(material.ambient.x, material.ambient.y, material.ambient.z, material.alpha);
	d3dMaterial.Diffuse = Color4F(material.diffuse.x, material.diffuse.y, material.diffuse.z, material.alpha);
	d3dMaterial.Specular = Color4F(material.specular.x, material.specular.y, material.specular.z, material.alpha);
	d3dMaterial.Emissive = Color4F(material.emissive.x, material.emissive.y, material.emissive.z, material.alpha);
	d3dMaterial.Power = material.shininess;

	this->addD3DMaterial(d3dMaterial);
}

void MeshRenderer::addTexture(LPDIRECT3DTEXTURE9 texture)
{
	_textures.push_back(texture);
}

void MeshRenderer::setNumberOfSubsets(unsigned int number)
{
	_numSubset = number;
}

unsigned int MeshRenderer::getNumberOfSubsets()
{
	return _numSubset;
}
