#include "MeshFileReader.h"
#include "Material.h"

USING_NS_TINY;

MeshFileReader::MeshFileReader(std::string filePath) : FileReader(filePath),
	_currentMeshIndex(0)
{
}

MeshFileReader::~MeshFileReader()
{
}

bool MeshFileReader::readData()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(this->getFilePath(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return false;
	}

	this->processNode(scene->mRootNode, scene);

	return true;
}

const std::vector<Material*>& MeshFileReader::getMaterials()
{
	return _materials;
}

const std::vector<Mesh::Vertex>& MeshFileReader::getVertices()
{
	return _vertices;
}

const std::vector<DWORD>& MeshFileReader::getIndices()
{
	return _indices;
}

const std::vector<DWORD>& MeshFileReader::getAttributes()
{
	return _attributes;
}

unsigned int MeshFileReader::getNumMesh()
{
	return _currentMeshIndex;
}

void MeshFileReader::processNode(aiNode * node, const aiScene * scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->processMesh(mesh, scene);

		_currentMeshIndex++;
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

void MeshFileReader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	int offset = _vertices.size();

	// vertex
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Mesh::Vertex vertex;

		// position
		vertex.position = Vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		// normals
		if (mesh->mNormals)
		{
			vertex.normal = Vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		// textcoords
		if (mesh->mTextureCoords[0])
		{
			vertex.texcoord = Vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertex.texcoord = Vec2(0.0f, 0.0f);
		}

		_vertices.push_back(vertex);
	}

	// indices
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// đọc indices từng face
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			_indices.push_back(offset + face.mIndices[j]);
		}

		_attributes.push_back(_currentMeshIndex);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		auto mat = this->processMaterial(material, mesh);
		_materials.push_back(mat);
	}
}

Material* MeshFileReader::processMaterial(aiMaterial * material, aiMesh * mesh)
{
	auto mat = new Material();

	aiColor3D color(0.0f, 0.0f, 0.0f);
	if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, color))
	{
		mat->ambient = Vec3(color.r, color.g, color.b);
	}

	if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
	{
		mat->diffuse = Vec3(color.r, color.g, color.b);
	}

	if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, color))
	{
		mat->specular = Vec3(color.r, color.g, color.b);
		mat->hasSpecular = true;
	}

	if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_EMISSIVE, color))
	{
		mat->emissive = Vec3(color.r, color.g, color.b);
	}

	float fValue = 0.0f;
	if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS_STRENGTH, fValue))
	{
		mat->shininess = fValue;
	}

	if (AI_SUCCESS == material->Get(AI_MATKEY_OPACITY, fValue))
	{
		mat->alpha = fValue;
	}

	// load diffuse texture
	// test load 1 cái
	for (size_t i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++)
	{
		aiString texturePath;
		material->GetTexture(aiTextureType_DIFFUSE, i, &texturePath);
		mat->texturePath = _relativePath + texturePath.C_Str();
	}

	return mat;
}
