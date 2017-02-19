#include "MeshLoader.h"
#include "Mesh.h"
#include "Material.h"
#include "..\Utils.h"
#include "OBJFileReader.h"

#include <fstream>

USING_NS_TINY;

D3DVERTEXELEMENT9 MeshLoader::VERTEX_DECL[] =
{
	{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,   0 },
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD, 0 },
	D3DDECL_END()
};

bool MeshLoader::loadFromXFile(Mesh* target, const char* filePath, LPDIRECT3DDEVICE9 device)
{
	ID3DXBuffer* adjBuffer = NULL;
	ID3DXBuffer* mtrlBuffer = NULL;
	LPD3DXMESH mesh;
	DWORD numMtrls = 0;

	HRESULT result = D3DXLoadMeshFromX(
		filePath,
		D3DXMESH_MANAGED,
		device,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&mesh
	);

	if (FAILED(result))
	{
		OutputDebugString("Load mesh from X FAILED!!!");
		return false;
	}

	if (mtrlBuffer != NULL && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (size_t i = 0; i < numMtrls; i++)
		{
			// ko có ambient nên phải gán lại
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			LPDIRECT3DTEXTURE9 texture = NULL;

			// check coi có file texture ko
			if (mtrls[i].pTextureFilename != 0)
			{
				D3DXCreateTextureFromFile(device, mtrls[i].pTextureFilename, &texture);
			}

			// add vào target
			target->addD3DMaterial(mtrls[i].MatD3D, texture);
		}

		mtrlBuffer->Release();
	}

	if (adjBuffer != NULL)
	{
		result = mesh->OptimizeInplace(
			D3DXMESHOPT_ATTRSORT |
			D3DXMESHOPT_COMPACT |
			D3DXMESHOPT_VERTEXCACHE,
			(DWORD*)adjBuffer->GetBufferPointer(),
			0, 0, 0);

		if (FAILED(result))
		{
			OutputDebugString("OptimizeInplace() FAILED!!!");

			// release
			adjBuffer->Release();
			mesh->Release();

			return false;
		}

		adjBuffer->Release();
	}

	// tính normal vector
	if (!(mesh->GetFVF() & D3DFVF_NORMAL))
	{
		LPD3DXMESH tempMesh;
		result = mesh->CloneMeshFVF(D3DXMESH_MANAGED, mesh->GetFVF() | D3DFVF_NORMAL, device, &tempMesh);

		if (FAILED(result))
		{
			OutputDebugString("CloneMeshFVF() FAILED!!!");
			mesh->Release();

			return false;
		}

		result = D3DXComputeNormals(tempMesh, 0);

		if (FAILED(result))
		{
			OutputDebugString("D3DXComputeNormals() FAILED!!!");

			mesh->Release();
			tempMesh->Release();

			return false;
		}

		// xóa cái cũ gán cái mới tính vào
		mesh->Release();
		mesh = tempMesh;
	}

	target->setD3DMesh(mesh);

	return true;
}

bool MeshLoader::loadFromOBJFile(Mesh * target, const char * filePath, LPDIRECT3DDEVICE9 device)
{
	auto reader = new OBJFileReader(filePath);
	reader->readData();

	auto indices = reader->getIndices();
	auto vertices = reader->getVertices();
	auto attributes = reader->getAttributes();

	// Create the encapsulated mesh
	ID3DXMesh* pMesh = NULL;

	D3DXCreateMesh(indices.size() / 3, vertices.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_DECL,
		device, &pMesh);

	// Copy the vertex data
	Mesh::Vertex* pVertex;
	pMesh->LockVertexBuffer(0, (void**)&pVertex);
	memcpy(pVertex, vertices.data(), vertices.size() * sizeof(Mesh::Vertex));
	pMesh->UnlockVertexBuffer();
	vertices.clear();

	// Copy the index data
	DWORD* pIndex;
	pMesh->LockIndexBuffer(0, (void**)&pIndex);
	memcpy(pIndex, indices.data(), indices.size() * sizeof(DWORD));
	pMesh->UnlockIndexBuffer();
	indices.clear();

	// Copy the attribute data
	DWORD* pSubset;
	pMesh->LockAttributeBuffer(0, &pSubset);
	memcpy(pSubset, attributes.data(), attributes.size() * sizeof(DWORD));
	pMesh->UnlockAttributeBuffer();
	attributes.clear();

	// Optimize mesh
	DWORD* aAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
	if (aAdjacency == NULL)
	{
		return false;
	}

	pMesh->GenerateAdjacency(1e-6f, aAdjacency);
	pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, aAdjacency, NULL, NULL, NULL);

	delete[] aAdjacency;

	// tính normal vector
	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		LPD3DXMESH tempMesh;
		pMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMesh->GetFVF() | D3DFVF_NORMAL, device, &tempMesh);

		D3DXComputeNormals(tempMesh, 0);

		pMesh->Release();
		pMesh = tempMesh;
	}

	// set mesh target
	target->setD3DMesh(pMesh);

	// create textures
	auto materials = reader->getMaterials();
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
				auto path = reader->getRelativePath() + pMaterial->texturePath;
				D3DXCreateTextureFromFile(device, path.c_str(), &(pMaterial->texture));
			}
		}
	}

	for (size_t i = 0; i < materials.size(); i++)
	{
		target->addMaterial(materials[i]);
	}

	delete reader;

	return true;
}
