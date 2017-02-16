#pragma once

#include <vector>
#include "..\TinyDefinitions.h"
#include "Object.h"

NS_TINY_BEGIN

class MeshRenderer;

class Mesh : public Object
{
public:
	Mesh();
	~Mesh();

	virtual bool init() override;

	virtual bool initWithXFile(const char* filePath);

	virtual bool initWithOBJFile(const char* filePath);

	virtual void draw() override;

	virtual void setD3DMesh(LPD3DXMESH mesh);

	virtual void addMaterial(D3DMATERIAL9 material);

	virtual void addTexture(LPDIRECT3DTEXTURE9 texture);

private:
	LPDIRECT3DDEVICE9 _device;
	MeshRenderer* _renderer;

};

NS_TINY_END