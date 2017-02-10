#pragma once

#include "Tiny3D.h"

USING_NS_TINY;

class Cube : public Object
{
public:
	Cube();
	~Cube();

	bool init(const char* filePath);
	void draw();

	struct CubeVertex
	{
		FLOAT x, y, z;
		FLOAT nx, ny, nz;
		FLOAT u, v;
	};

	static DWORD CUBE_FVF;

private:
	LPDIRECT3DDEVICE9 _device;
	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer;
	LPDIRECT3DINDEXBUFFER9 _indicesBuffer;
	LPDIRECT3DTEXTURE9 _texture;

	Transform* _transform;

	bool initVertexBuffer();
	bool initIndicesBuffer();
};