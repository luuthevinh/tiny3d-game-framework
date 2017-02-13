#pragma once

#include <vector>
#include "..\TinyDefinitions.h"
#include "..\base\Renderer.h"

NS_TINY_BEGIN

class MeshRenderer : public Renderer
{
public:
	MeshRenderer();
	~MeshRenderer();

	virtual void draw() override;
	virtual void release() override;

	void setD3DMesh(LPD3DXMESH mesh);

	void addMaterial(D3DMATERIAL9 material);

	void addTexture(LPDIRECT3DTEXTURE9 texture);

private:
	LPD3DXMESH _meshD3D;

	std::vector<D3DMATERIAL9> _materials;
	std::vector<LPDIRECT3DTEXTURE9> _textures;
};

NS_TINY_END