#include "MeshLoader.h"
#include "Mesh.h"
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

			// add vào vector
			target->addMaterial(mtrls[i].MatD3D);

			// check coi có file texture ko
			if (mtrls[i].pTextureFilename != 0)
			{
				LPDIRECT3DTEXTURE9 texture = 0;
				D3DXCreateTextureFromFile(device, mtrls[i].pTextureFilename, &texture);

				target->addTexture(texture);
			}
			else
			{
				// ko có texture cho subset này
				target->addTexture(0);
			}
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
	auto reader = new OBJFileReader();
	reader->readData(filePath);

	auto indices = reader->getIndices();
	auto vertices = reader->getVertices();
	auto attributes = reader->getAttributes();

	// Create the encapsulated mesh
	ID3DXMesh* pMesh = NULL;

	D3DXCreateMesh(indices.size() / 3, vertices.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_DECL,
		device, &pMesh);

	// Copy the vertex data
	VERTEX* pVertex;
	pMesh->LockVertexBuffer(0, (void**)&pVertex);
	memcpy(pVertex, vertices.data(), vertices.size() * sizeof(VERTEX));
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
				if (0 == wcscmp(pCur->texturePath, pMaterial->texturePath))
				{
					bFound = true;
					pMaterial->texture = pCur->texture;
					break;
				}
			}

			// Not found, load the texture
			if (!bFound)
			{
				auto path = reader->getRelativePath() + Utils::toString(pMaterial->texturePath);
				D3DXCreateTextureFromFile(device, path.c_str(), &(pMaterial->texture));
			}
		}
	}

	// set mesh target
	target->setD3DMesh(pMesh);

	for (size_t i = 0; i < materials.size(); i++)
	{
		D3DMATERIAL9 mat = Utils::WHITE_MATERIAL;

		mat.Ambient = Color4F(materials[i]->ambient.x, materials[i]->ambient.y, materials[i]->ambient.z, materials[i]->alpha);
		mat.Diffuse = Color4F(materials[i]->diffuse.x, materials[i]->diffuse.y, materials[i]->diffuse.z, materials[i]->alpha);
		mat.Specular = Color4F(materials[i]->specular.x, materials[i]->specular.y, materials[i]->specular.z, materials[i]->alpha);
		mat.Power = materials[i]->shininess;

		target->addMaterial(mat);
		target->addTexture(materials[i]->texture);
	}

	delete reader;

	return true;
}

bool MeshLoader::loadFromOBJFileX(Mesh* target, const char* filePath, LPDIRECT3DDEVICE9 device)
{
	std::vector<D3DXVECTOR3> positions;
	std::vector<D3DXVECTOR2> texCoords;
	std::vector<D3DXVECTOR3> normals;

	std::vector<Material*> materials;
	std::vector<VERTEX> vertices;
	std::vector<DWORD> indices;
	std::vector<DWORD> attributes;

	WCHAR strMaterialFilename[MAX_PATH] = { 0 };

	auto splitPath = Utils::splitString(filePath, '\\');

	std::string relativePath(filePath);
	relativePath.erase(relativePath.end() - strlen(splitPath.at(splitPath.size() - 1).c_str()) , relativePath.end());

	Material* material = new Material();
	if (material == NULL)
		return false;

	wcscpy_s(material->name, MAX_PATH - 1, L"default");
	materials.push_back(material);

	DWORD curSubset = 0;

	// File input
	WCHAR strCommand[256] = { 0 };
	std::wifstream InFile(filePath);
	if (!InFile)
	{
		auto output = "can't open file .obj: " + std::string(filePath);
		OutputDebugString(output.c_str());
		return false;
	}
	
	for (; ; )
	{
		InFile >> strCommand;
		if (!InFile)
			break;

		if (0 == wcscmp(strCommand, L"#"))
		{
			// Comment
		}
		else if (0 == wcscmp(strCommand, L"v"))
		{
			// Vertex Position
			float x, y, z;
			InFile >> x >> y >> z;
			positions.push_back(Vec3(x, y, z));
		}
		else if (0 == wcscmp(strCommand, L"vt"))
		{
			// Vertex TexCoord
			float u, v;
			InFile >> u >> v;
			texCoords.push_back(Vec2(u, v));
		}
		else if (0 == wcscmp(strCommand, L"vn"))
		{
			// Vertex Normal
			float x, y, z;
			InFile >> x >> y >> z;
			normals.push_back(Vec3(x, y, z));
		}
		else if (0 == wcscmp(strCommand, L"f"))
		{
			// Face
			UINT iPosition, iTexCoord, iNormal;
			VERTEX vertex;

			for (UINT iFace = 0; iFace < 3; iFace++)
			{
				ZeroMemory(&vertex, sizeof(VERTEX));

				// OBJ xài mảng bắt đầu từ 1
				InFile >> iPosition;
				vertex.position = positions[iPosition - 1];

				if ('/' == InFile.peek())
				{
					InFile.ignore();

					if ('/' != InFile.peek())
					{
						// Optional texture coordinate
						InFile >> iTexCoord;
						vertex.texcoord = texCoords[iTexCoord - 1];
					}

					if ('/' == InFile.peek())
					{
						InFile.ignore();

						// Optional vertex normal
						InFile >> iNormal;
						vertex.normal = normals[iNormal - 1];
					}
				}

				// If a duplicate vertex doesn't exist, add this vertex to the Vertices
				// list. Store the index in the Indices array. The Vertices and Indices
				// lists will eventually become the Vertex Buffer and Index Buffer for
				// the mesh.
				// DWORD index = AddVertex(iPosition, &vertex);

				DWORD index = (DWORD)-1;

				// find vertex
				auto result = vertices.end();
				if (iPosition < vertices.size())
				{
					result = std::find(vertices.begin(), vertices.end(), vertex);
				}
				
				if (result != vertices.end())
				{
					index = result - vertices.begin();
				}
				else
				{
					vertices.push_back(vertex);
					index = vertices.size() - 1;
				}


				if (index == (DWORD)-1)
				{
					OutputDebugString("index == (DWORD)-1");
					return false;
				}

				indices.push_back(index);
			}

			attributes.push_back(curSubset);
		}
		else if (0 == wcscmp(strCommand, L"mtllib"))
		{
			// Material library
			InFile >> strMaterialFilename;
		}
		else if (0 == wcscmp(strCommand, L"usemtl"))
		{
			// Material
			WCHAR strName[MAX_PATH] = { 0 };
			InFile >> strName;

			bool bFound = false;
			for (int iMaterial = 0; iMaterial < materials.size(); iMaterial++)
			{
				Material* pCurMaterial = materials.at(iMaterial);
				if (0 == wcscmp(pCurMaterial->name, strName))
				{
					bFound = true;
					curSubset = iMaterial;
					break;
				}
			}

			if (!bFound)
			{
				material = new Material();
				if (material == NULL)
					return false;

				curSubset = materials.size();

				wcscpy_s(material->name, MAX_PATH - 1, strName);

				materials.push_back(material);
			}
		}
		else
		{
			// Unimplemented or unrecognized command
		}

		InFile.ignore(1000, '\n');
	}

	// Cleanup
	InFile.close();

	// If an associated material file was found, read that in as well.
	if (strMaterialFilename[0])
	{
		auto path = relativePath + Utils::toString(strMaterialFilename);

		// File input
		WCHAR strCommand[256] = { 0 };
		std::wifstream InFile(path);
		if (!InFile)
		{
			auto output = "cannot open material: " + path;
			OutputDebugString(output.c_str());
			return false;
		}

		Material* pMaterial = NULL;

		for (; ; )
		{
			InFile >> strCommand;
			if (!InFile)
				break;

			if (0 == wcscmp(strCommand, L"newmtl"))
			{
				// Switching active materials
				WCHAR strName[MAX_PATH] = { 0 };
				InFile >> strName;

				pMaterial = NULL;
				for (int i = 0; i < materials.size(); i++)
				{
					Material* pCurMaterial = materials.at(i);
					if (0 == wcscmp(pCurMaterial->name, strName))
					{
						pMaterial = pCurMaterial;
						break;
					}
				}
			}

			// The rest of the commands rely on an active material
			if (pMaterial == NULL)
				continue;

			if (0 == wcscmp(strCommand, L"#"))
			{
				// Comment
			}
			else if (0 == wcscmp(strCommand, L"Ka"))
			{
				// Ambient color
				float r, g, b;
				InFile >> r >> g >> b;
				pMaterial->ambient = D3DXVECTOR3(r, g, b);
			}
			else if (0 == wcscmp(strCommand, L"Kd"))
			{
				// Diffuse color
				float r, g, b;
				InFile >> r >> g >> b;
				pMaterial->diffuse = D3DXVECTOR3(r, g, b);
			}
			else if (0 == wcscmp(strCommand, L"Ks"))
			{
				// Specular color
				float r, g, b;
				InFile >> r >> g >> b;
				pMaterial->specular = D3DXVECTOR3(r, g, b);
			}
			else if (0 == wcscmp(strCommand, L"d") ||
				0 == wcscmp(strCommand, L"Tr"))
			{
				// Alpha
				InFile >> pMaterial->alpha;
			}
			else if (0 == wcscmp(strCommand, L"Ns"))
			{
				// Shininess
				int nShininess;
				InFile >> nShininess;
				pMaterial->shininess = nShininess;
			}
			else if (0 == wcscmp(strCommand, L"illum"))
			{
				// Specular on/off
				int illumination;
				InFile >> illumination;
				pMaterial->hasSpecular = (illumination == 2);
			}
			else if (0 == wcscmp(strCommand, L"map_Kd"))
			{
				// Texture
				InFile >> pMaterial->texturePath;
			}

			else
			{
				// Unimplemented or unrecognized command
			}

			InFile.ignore(1000, L'\n');
		}

		InFile.close();
	}

	// Load material textures
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
				if (0 == wcscmp(pCur->texturePath, pMaterial->texturePath))
				{
					bFound = true;
					pMaterial->texture = pCur->texture;
					break;
				}
			}

			// Not found, load the texture
			if (!bFound)
			{
				auto path = relativePath + Utils::toString(pMaterial->texturePath);
				D3DXCreateTextureFromFile(device, path.c_str(), &(pMaterial->texture));
			}
		}
	}

	// Create the encapsulated mesh
	ID3DXMesh* pMesh = NULL;
	D3DXCreateMesh(indices.size() / 3, vertices.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_DECL,
		device, &pMesh);

	// Copy the vertex data
	VERTEX* pVertex;
	pMesh->LockVertexBuffer(0, (void**)&pVertex);
	memcpy(pVertex, vertices.data(), vertices.size() * sizeof(VERTEX));
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

	for (size_t i = 0; i < materials.size(); i++)
	{
		D3DMATERIAL9 mat = Utils::WHITE_MATERIAL;

		mat.Ambient = Color4F(materials[i]->ambient.x, materials[i]->ambient.y, materials[i]->ambient.z, materials[i]->alpha);
		mat.Diffuse = Color4F(materials[i]->diffuse.x, materials[i]->diffuse.y, materials[i]->diffuse.z, materials[i]->alpha);
		mat.Specular = Color4F(materials[i]->specular.x, materials[i]->specular.y, materials[i]->specular.z, materials[i]->alpha);
		mat.Power = materials[i]->shininess;

		target->addMaterial(mat);
		target->addTexture(materials[i]->texture);
	}

	return true;
}