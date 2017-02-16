#pragma once

#include "..\TinyDefinitions.h"

NS_TINY_BEGIN

class Mesh;

class Material
{
public:
	Material()
	{
		ZeroMemory(this, sizeof(Material));

		this->ambient = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
		this->diffuse = D3DXVECTOR3(0.8f, 0.8f, 0.8f);
		this->specular = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		this->shininess = 0;
		this->alpha = 1.0f;
		this->hasSpecular = false;
		this->texture = NULL;
	}

	WCHAR   name[MAX_PATH];

	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;

	int shininess;
	float alpha;

	bool hasSpecular;

	WCHAR   texturePath[MAX_PATH];
	IDirect3DTexture9* texture;

	D3DXHANDLE hTechnique;
};

class MeshLoader
{
public:
	static bool loadFromXFile(Mesh* target, const char* filePath, LPDIRECT3DDEVICE9 device);
	
	static bool loadFromOBJFile(Mesh* target, const char* filePath, LPDIRECT3DDEVICE9 device);
	static bool loadFromOBJFileX(Mesh* target, const char* filePath, LPDIRECT3DDEVICE9 device);

	static D3DVERTEXELEMENT9 VERTEX_DECL[];

	struct VERTEX
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 texcoord;

		bool operator==(const VERTEX& other) {
			if (this->position != other.position)
				return false;

			if (this->normal != other.normal)
				return false;

			if (this->texcoord != other.texcoord)
				return false;

			return true;
		}
	};

private:
	MeshLoader() { }
};

NS_TINY_END