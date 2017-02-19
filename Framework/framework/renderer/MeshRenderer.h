#pragma once

#include <vector>
#include "..\TinyDefinitions.h"
#include "..\base\Renderer.h"

NS_TINY_BEGIN

class Material;

class MeshRenderer : public Renderer
{
public:
	MeshRenderer();
	~MeshRenderer();

	virtual void draw() override;
	virtual void release() override;

	void setD3DMesh(LPD3DXMESH mesh);

	void addD3DMaterial(const D3DMATERIAL9& material);
	void addMaterial(const Material& material);

	void addTexture(LPDIRECT3DTEXTURE9 texture);

	void setNumberOfSubsets(unsigned int number);
	unsigned int getNumberOfSubsets();

private:
	LPD3DXMESH _meshD3D;

	std::vector<D3DMATERIAL9> _materials;
	std::vector<LPDIRECT3DTEXTURE9> _textures;

	unsigned int _numSubset;
};

NS_TINY_END