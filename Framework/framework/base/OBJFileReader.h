#pragma once

#include <vector>
#include <string>

#include "..\TinyDefinitions.h"
#include "MeshLoader.h"

NS_TINY_BEGIN

class OBJFileReader
{
public:
	OBJFileReader();
	~OBJFileReader();

	bool readData(const char* filePath);

	const std::vector<Material*>& getMaterials();
	const std::vector<MeshLoader::VERTEX>& getVertices();
	const std::vector<DWORD>& getIndices();
	const std::vector<DWORD>& getAttributes();

	const char* getRelativePath();

private:
	std::vector<Vec3> _positions;
	std::vector<Vec2> _texCoords;
	std::vector<Vec3> _normals;

	std::vector<Material*> _materials;
	std::vector<MeshLoader::VERTEX> _vertices;
	std::vector<DWORD> _indices;
	std::vector<DWORD> _attributes;

	std::string _relativePath;

	DWORD addVertex(UINT position, const MeshLoader::VERTEX& vertex);
	bool readMaterialData(const char* materialPath);
};

NS_TINY_END