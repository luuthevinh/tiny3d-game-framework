#pragma once

#include "..\TinyDefinitions.h"
#include "..\base\Renderer.h"

NS_TINY_BEGIN

class Material;

class CubeRenderer : public tiny::Renderer
{
public:
	CubeRenderer();
	~CubeRenderer();

	virtual bool init() override;
	virtual void draw(LPDIRECT3DVERTEXBUFFER9 vertex, LPDIRECT3DINDEXBUFFER9 index);

	void setTexture(LPDIRECT3DTEXTURE9 texture);
	void setMaterial(const Material& material);

private:
	LPDIRECT3DTEXTURE9 _texture;
	D3DMATERIAL9 _material;

};

NS_TINY_END