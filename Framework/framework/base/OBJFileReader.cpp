#include "OBJFileReader.h"
#include "..\Utils.h"
#include "MeshLoader.h"
#include <fstream>

USING_NS_TINY;

OBJFileReader::OBJFileReader()
{
}

OBJFileReader::~OBJFileReader()
{
	while (!_materials.empty())
	{
		SAFE_DELETE(_materials.back());
		_materials.pop_back();
	}
}

bool OBJFileReader::readData(const char* filePath)
{
	auto splitPath = Utils::splitString(filePath, '\\');

	_relativePath = filePath;
	_relativePath.erase(_relativePath.end() - strlen(splitPath.at(splitPath.size() - 1).c_str()), _relativePath.end());

	WCHAR strMaterialFilename[MAX_PATH] = { 0 };
	DWORD curSubset = 0;

	// add default material
	Material* material = new Material();
	if (material == nullptr)
		return false;

	wcscpy_s(material->name, MAX_PATH - 1, L"default");
	_materials.push_back(material);

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
			_positions.push_back(Vec3(x, y, z));
		}
		else if (0 == wcscmp(strCommand, L"vt"))
		{
			// Vertex TexCoord
			float u, v;
			InFile >> u >> v;
			_texCoords.push_back(Vec2(u, v));
		}
		else if (0 == wcscmp(strCommand, L"vn"))
		{
			// Vertex Normal
			float x, y, z;
			InFile >> x >> y >> z;
			_normals.push_back(Vec3(x, y, z));
		}
		else if (0 == wcscmp(strCommand, L"f"))
		{
			// Face
			UINT iPosition, iTexCoord, iNormal;
			MeshLoader::VERTEX vertex;

			for (UINT iFace = 0; iFace < 3; iFace++)
			{
				ZeroMemory(&vertex, sizeof(MeshLoader::VERTEX));

				// OBJ xài mảng bắt đầu từ 1
				InFile >> iPosition;
				vertex.position = _positions[iPosition - 1];

				if ('/' == InFile.peek())
				{
					InFile.ignore();

					if ('/' != InFile.peek())
					{
						// Optional texture coordinate
						InFile >> iTexCoord;
						vertex.texcoord = _texCoords[iTexCoord - 1];
					}

					if ('/' == InFile.peek())
					{
						InFile.ignore();

						// Optional vertex normal
						InFile >> iNormal;
						vertex.normal = _normals[iNormal - 1];
					}
				}

				// add vertex vào danh sách vertices, nếu có rồi thì lấy index nó thôi, không add trùng
				DWORD index = (DWORD)-1;
				index = this->addVertex(iPosition, vertex);

				if (index == (DWORD)-1)
				{
					OutputDebugString("error in adding vertex into _vertices");
					return false;
				}

				_indices.push_back(index);
			}

			_attributes.push_back(curSubset);
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
			for (int iMaterial = 0; iMaterial < _materials.size(); iMaterial++)
			{
				Material* pCurMaterial = _materials.at(iMaterial);
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

				curSubset = _materials.size();

				wcscpy_s(material->name, MAX_PATH - 1, strName);

				_materials.push_back(material);
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
		if (!this->readMaterialData(Utils::toString(strMaterialFilename).c_str()))
		{
			return false;
		}
	}

	return true;
}

const std::vector<Material*>& OBJFileReader::getMaterials()
{
	return _materials;
}

const std::vector<MeshLoader::VERTEX>& OBJFileReader::getVertices()
{
	return _vertices;
}

const std::vector<DWORD>& OBJFileReader::getIndices()
{
	return _indices;
}

const std::vector<DWORD>& OBJFileReader::getAttributes()
{
	return _attributes;
}

const char * OBJFileReader::getRelativePath()
{
	return _relativePath.c_str();
}

DWORD OBJFileReader::addVertex(UINT position, const MeshLoader::VERTEX & vertex)
{
	auto result = _vertices.end();

	if (position < _vertices.size())
	{
		result = std::find(_vertices.begin(), _vertices.end(), vertex);
	}

	if (result != _vertices.end())
	{
		return result - _vertices.begin();
	}

	_vertices.push_back(vertex);
	return _vertices.size() - 1;
}

bool OBJFileReader::readMaterialData(const char * materialPath)
{
	auto path = _relativePath + materialPath;

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
			for (int i = 0; i < _materials.size(); i++)
			{
				Material* pCurMaterial = _materials.at(i);
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

	return true;
}
