#pragma once

#include "..\TinyDefinitions.h"
#include "..\base\Object.h"

NS_TINY_BEGIN

class Cube : public Object
{
public:
	Cube();
	~Cube();

	bool init();
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

	bool initVertexBuffer();
	bool initIndicesBuffer();
};

NS_TINY_END