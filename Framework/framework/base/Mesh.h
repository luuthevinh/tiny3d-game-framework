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

	virtual bool initWithXFile(const char* filePath);
	virtual void draw() override;

private:
	LPDIRECT3DDEVICE9 _device;
	MeshRenderer* _renderer;
};

NS_TINY_END