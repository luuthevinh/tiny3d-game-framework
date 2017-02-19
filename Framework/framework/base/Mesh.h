#pragma once

#include <vector>
#include "..\TinyDefinitions.h"
#include "Object.h"

NS_TINY_BEGIN

class MeshRenderer;
class Material;
class MeshFileReader;

class Mesh : public Object
{
public:
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 texcoord;

		bool operator==(const Vertex& other) {
			if (this->position != other.position)
				return false;

			if (this->normal != other.normal)
				return false;

			if (this->texcoord != other.texcoord)
				return false;

			return true;
		}
	};

	Mesh();
	~Mesh();

	virtual bool init() override;

	virtual bool initWithFile(const char* filePath);

	virtual bool initWithXFile(const char* filePath);

	virtual bool initWithOBJFile(const char* filePath);

	virtual bool initWithData(const std::vector<Vertex>& vertices, const std::vector<DWORD>& indices, const std::vector<Material*>& materials);

	virtual void draw() override;

	virtual void setD3DMesh(LPD3DXMESH mesh);
	virtual void addMaterial(Material* material);
	virtual void addD3DMaterial(const D3DMATERIAL9& material, LPDIRECT3DTEXTURE9 texture = nullptr);

	void copyVerticesData(ID3DXMesh* mesh, const std::vector<Vertex>& vertices);
	void copyIndicesData(ID3DXMesh* mesh, const std::vector<DWORD>& indices);
	void copyAttributesData(ID3DXMesh* mesh, const std::vector<DWORD>& attributes);
	void optimizeMesh(ID3DXMesh* mesh);
	void calculateNormal(ID3DXMesh& mesh);
	void loadMaterialTextures(const std::vector<Material*>& materials);

private:
	LPDIRECT3DDEVICE9 _device;
	MeshRenderer* _renderer;

	bool initD3DMesh(MeshFileReader* reader);
};

NS_TINY_END