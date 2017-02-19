#pragma once

#include "..\TinyDefinitions.h"

NS_TINY_BEGIN

class Mesh;
class Material;

class MeshLoader
{
public:
	static bool loadFromXFile(Mesh* target, const char* filePath, LPDIRECT3DDEVICE9 device);

	static bool loadFromOBJFile(Mesh* target, const char* filePath, LPDIRECT3DDEVICE9 device);
	
	static D3DVERTEXELEMENT9 VERTEX_DECL[];

	MeshLoader() { }

private:
	
};

NS_TINY_END