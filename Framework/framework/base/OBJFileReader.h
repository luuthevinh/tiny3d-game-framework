#pragma once

#include <vector>
#include <string>

#include "..\TinyDefinitions.h"
#include "Mesh.h"
#include "FileReader.h"

NS_TINY_BEGIN

class OBJFileReader : public FileReader
{
public:
	OBJFileReader(const char* filePath);
	~OBJFileReader();

	bool readData();

	const std::vector<Material*>& getMaterials();
	const std::vector<Mesh::Vertex>& getVertices();
	const std::vector<DWORD>& getIndices();
	const std::vector<DWORD>& getAttributes();

	unsigned int getNumberOfSubsets();

private:
	std::vector<Vec3> _positions;
	std::vector<Vec2> _texCoords;
	std::vector<Vec3> _normals;

	std::vector<Material*> _materials;
	std::vector<Mesh::Vertex> _vertices;
	std::vector<DWORD> _indices;
	std::vector<DWORD> _attributes;

	unsigned int _numSubsets;

	DWORD addVertex(UINT position, const Mesh::Vertex& vertex);
	bool readMaterialData(const char* materialPath);
};

NS_TINY_END