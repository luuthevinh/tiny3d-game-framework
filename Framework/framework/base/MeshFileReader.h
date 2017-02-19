#pragma once

#include <vector>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "..\TinyDefinitions.h"
#include "FileReader.h"
#include "Mesh.h"

NS_TINY_BEGIN

class Material;

class MeshFileReader : public FileReader
{
public:
	MeshFileReader(std::string filePath);
	~MeshFileReader();

	bool readData();

	const std::vector<Material*>& getMaterials();
	const std::vector<Mesh::Vertex>& getVertices();
	const std::vector<DWORD>& getIndices();
	const std::vector<DWORD>& getAttributes();

	unsigned int getNumMesh();

protected:
	std::vector<Material*> _materials;
	std::vector<Mesh::Vertex> _vertices;
	std::vector<DWORD> _indices;
	std::vector<DWORD> _attributes;

	unsigned int _currentMeshIndex;

	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	Material* processMaterial(aiMaterial* material, aiMesh* mesh);
};

NS_TINY_END