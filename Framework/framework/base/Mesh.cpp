#include "Mesh.h"
#include "Transform.h"
#include "..\renderer\MeshRenderer.h"
#include "..\SceneManager.h"

USING_NS_TINY;

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

bool Mesh::initWithXFile(const char* filePath)
{
	if (!Object::init())
	{
		return false;
	}

	_device = SceneManager::getInstance()->getDevice()->getDirec3DDevice();

	_renderer = new MeshRenderer();
	_renderer->init();

	this->addComponent(Component::RENDERDER, _renderer);

	ID3DXBuffer* adjBuffer = NULL;
	ID3DXBuffer* mtrlBuffer = NULL;
	LPD3DXMESH mesh;
	DWORD numMtrls = 0;

	HRESULT result = D3DXLoadMeshFromX(
		filePath,
		D3DXMESH_MANAGED,
		_device,
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
			_renderer->addMaterial(mtrls[i].MatD3D);

			// check coi có file texture ko
			if (mtrls[i].pTextureFilename != 0)
			{
				LPDIRECT3DTEXTURE9 texture = 0;
				D3DXCreateTextureFromFile(_device, mtrls[i].pTextureFilename, &texture);

				_renderer->addTexture(texture);
			}
			else
			{
				// ko có texture cho subset này
				_renderer->addTexture(0);
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
			return false;
		}

		adjBuffer->Release();
	}

	// tính normal vector
	if (!(mesh->GetFVF() & D3DFVF_NORMAL))
	{
		LPD3DXMESH tempMesh;
		result = mesh->CloneMeshFVF(D3DXMESH_MANAGED, mesh->GetFVF() | D3DFVF_NORMAL, _device, &tempMesh);

		if (FAILED(result))
		{
			OutputDebugString("CloneMeshFVF() FAILED!!!");
			return false;
		}

		result = D3DXComputeNormals(tempMesh, 0);

		if (FAILED(result))
		{
			OutputDebugString("D3DXComputeNormals() FAILED!!!");
			return false;
		}

		// xóa cái cũ gán cái mới tính vào
		mesh->Release();
		mesh = tempMesh;
	}

	_renderer->setD3DMesh(mesh);

	return true;
}

void Mesh::draw()
{
	_renderer->draw();
}
