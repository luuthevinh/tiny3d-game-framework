#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include "..\renderer\MeshRenderer.h"
#include "..\SceneManager.h"

#include "MeshLoader.h"
#include "OBJFileReader.h"
#include "MeshFileReader.h"

USING_NS_TINY;

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

bool Mesh::init()
{
	if (!Object::init())
	{
		return false;
	}

	_device = SceneManager::getInstance()->getDevice()->getDirec3DDevice();

	// create renderer component
	_renderer = new MeshRenderer();
	_renderer->init();
	//_renderer->setFillMode(Renderer::FillMode::WIREFRAME);

	this->addComponent(Component::RENDERDER, _renderer);

	return true;
}

bool Mesh::initWithFile(const char * filePath)
{
	if (!this->init())
	{
		return false;
	}

	auto reader = new MeshFileReader(filePath);
	if (!reader->readData())
		return false;

	this->initD3DMesh(reader);

	return true;
}

bool Mesh::initWithXFile(const char* filePath)
{
	if (!this->init())
	{
		return false;
	}

	// load from x file
	if (!MeshLoader::loadFromXFile(this, filePath, _device))
	{
		return false;
	}

	return true;
}

bool Mesh::initWithOBJFile(const char * filePath)
{
	if (!this->init())
	{
		return false;
	}

	auto reader = new OBJFileReader(filePath);
	reader->readData();

	auto indices = reader->getIndices();
	auto vertices = reader->getVertices();
	auto attributes = reader->getAttributes();

	// Create mesh
	ID3DXMesh* pMesh = NULL;

	HRESULT result = D3DXCreateMesh(indices.size() / 3, vertices.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT, MeshLoader::VERTEX_DECL,
		_device, &pMesh);

	if (FAILED(result))
		return false;

	this->copyVerticesData(pMesh, vertices);
	this->copyIndicesData(pMesh, indices);
	this->copyAttributesData(pMesh, attributes);
	this->optimizeMesh(pMesh);
	this->calculateNormal(*pMesh);

	this->setD3DMesh(pMesh);
	_renderer->setNumberOfSubsets(reader->getNumberOfSubsets());

	this->loadMaterialTextures(reader->getMaterials());

	delete reader;

	return true;
}

bool Mesh::initWithData(const std::vector<Vertex>& vertices, const std::vector<unsigned long>& indices, const std::vector<Material*>& materials)
{
	if (!this->init())
	{
		return false;
	}

	// Create mesh
	ID3DXMesh* pMesh = NULL;

	HRESULT result = D3DXCreateMesh(indices.size() / 3, vertices.size(), 
		D3DXMESH_MANAGED | D3DXMESH_32BIT, MeshLoader::VERTEX_DECL, 
		_device, &pMesh);

	if (FAILED(result))
		return false;

	this->copyVerticesData(pMesh, vertices);
	this->copyIndicesData(pMesh, indices);
	this->optimizeMesh(pMesh);
	this->calculateNormal(*pMesh);

	this->setD3DMesh(pMesh);

	this->loadMaterialTextures(materials);

	return true;
}

void Mesh::draw()
{
	_renderer->draw();
}

void Mesh::setD3DMesh(LPD3DXMESH mesh)
{
	_renderer->setD3DMesh(mesh);
}

void Mesh::addMaterial(Material* material)
{
	_renderer->addMaterial(*material);

	_renderer->addTexture(material->texture);
}

void Mesh::addD3DMaterial(const D3DMATERIAL9 & material, LPDIRECT3DTEXTURE9 texture)
{
	_renderer->addD3DMaterial(material);
	_renderer->addTexture(texture);
}

void Mesh::copyVerticesData(ID3DXMesh* mesh, const std::vector<Vertex>& vertices)
{
	// Copy the vertex data
	Mesh::Vertex* pVertex;
	mesh->LockVertexBuffer(0, (void**)&pVertex);
	memcpy(pVertex, vertices.data(), vertices.size() * sizeof(Mesh::Vertex));
	mesh->UnlockVertexBuffer();
}

void Mesh::copyIndicesData(ID3DXMesh* mesh, const std::vector<DWORD>& indices)
{
	// Copy the index data
	DWORD* pIndex;
	mesh->LockIndexBuffer(0, (void**)&pIndex);
	memcpy(pIndex, indices.data(), indices.size() * sizeof(unsigned long));
	mesh->UnlockIndexBuffer();
}

void Mesh::copyAttributesData(ID3DXMesh* mesh, const std::vector<DWORD>& attributes)
{
	// Copy the attribute data
	DWORD* pSubset;
	mesh->LockAttributeBuffer(0, &pSubset);
	memcpy(pSubset, attributes.data(), attributes.size() * sizeof(DWORD));
	mesh->UnlockAttributeBuffer();
}

void Mesh::optimizeMesh(ID3DXMesh* mesh)
{
	// Optimize mesh
	DWORD* aAdjacency = new DWORD[mesh->GetNumFaces() * 3];
	if (aAdjacency == NULL)
	{
		return;
	}

	mesh->GenerateAdjacency(1e-6f, aAdjacency);
	mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, aAdjacency, NULL, NULL, NULL);

	delete[] aAdjacency;
}

void Mesh::calculateNormal(ID3DXMesh& mesh)
{
	if (!(mesh.GetFVF() & D3DFVF_NORMAL))
	{
		LPD3DXMESH tempMesh;
		mesh.CloneMeshFVF(D3DXMESH_MANAGED, mesh.GetFVF() | D3DFVF_NORMAL, _device, &tempMesh);

		D3DXComputeNormals(tempMesh, 0);

		mesh.Release();
		mesh = *tempMesh;
	}
}

void Mesh::loadMaterialTextures(const std::vector<Material*>& materials)
{
	for (int iMaterial = 0; iMaterial < materials.size(); iMaterial++)
	{
		Material* pMaterial = materials.at(iMaterial);
		if (pMaterial->texturePath[0])
		{
			// Avoid loading the same texture twice
			bool bFound = false;
			for (int x = 0; x < iMaterial; x++)
			{
				Material* pCur = materials.at(x);
				if (0 == pCur->texturePath.compare(pMaterial->texturePath))
				{
					bFound = true;
					pMaterial->texture = pCur->texture;
					break;
				}
			}

			// Not found, load the texture
			if (!bFound)
			{
				D3DXCreateTextureFromFile(_device, pMaterial->texturePath.c_str(), &(pMaterial->texture));
			}
		}

		// add to renderer
		this->addMaterial(materials[iMaterial]);
	}
}

bool Mesh::initD3DMesh(MeshFileReader* reader)
{
	// Create mesh
	ID3DXMesh* pMesh = NULL;

	auto indices = reader->getIndices();
	auto vertices = reader->getVertices();
	auto attributes = reader->getAttributes();
	auto materials = reader->getMaterials();

	HRESULT result = D3DXCreateMesh(indices.size() / 3, vertices.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT, MeshLoader::VERTEX_DECL,
		_device, &pMesh);

	if (FAILED(result))
		return false;

	this->copyVerticesData(pMesh, vertices);
	this->copyIndicesData(pMesh, indices);
	this->copyAttributesData(pMesh, attributes);
	this->optimizeMesh(pMesh);
	this->calculateNormal(*pMesh);

	this->loadMaterialTextures(materials);

	// set renderer
	_renderer->setD3DMesh(pMesh);
	_renderer->setNumberOfSubsets(reader->getNumMesh());
}
