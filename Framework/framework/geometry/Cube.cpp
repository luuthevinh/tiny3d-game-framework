#include "Cube.h"
#include "..\Direct3DDevice.h"
#include "..\SceneManager.h"
#include "..\base\Material.h"

#include "..\renderer\CubeRenderer.h"

USING_NS_TINY;

DWORD Cube::CUBE_FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

Cube::Cube()
{
}

Cube::~Cube()
{
	_vertexBuffer->Release();
	_indicesBuffer->Release();
}

bool Cube::init()
{
	Object::init();

	_device = SceneManager::getInstance()->getDevice()->getDirec3DDevice();
	
	if (!(this->initVertexBuffer() && this->initIndicesBuffer()))
	{
		return false;
	}

	auto renderer = new CubeRenderer();
	renderer->init();
	renderer->setMaterial(Material::WHITE_MATERIAL);
	this->addComponent(Component::RENDERDER, renderer);

	return true;
}

bool Cube::init(const char* filePath)
{
	Object::init();

	_device = SceneManager::getInstance()->getDevice()->getDirec3DDevice();

	if (!(this->initVertexBuffer() && this->initIndicesBuffer()))
	{
		return false;
	}

	LPDIRECT3DTEXTURE9 texture;
	D3DXCreateTextureFromFile(_device, filePath, &texture);

	_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	auto renderer = new CubeRenderer();
	renderer->init();
	renderer->setTexture(texture);
	renderer->setMaterial(Material::WHITE_MATERIAL);

	this->addComponent(Component::RENDERDER, renderer);

	return true;
}

void Cube::draw()
{
	auto renderer = (CubeRenderer*)this->getComponent(Component::RENDERDER);
	renderer->draw(_vertexBuffer, _indicesBuffer);
}

bool Cube::initVertexBuffer()
{
	// cube
	Cube::CubeVertex vertices[] =
	{
		{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },    // side 1
		{ 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },

		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f },    // side 2
		{ -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
		{ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f },

		{ -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },    // side 3
		{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },

		{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f },    // side 4
		{ 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f },
		{ -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f },

		{ 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },    // side 5
		{ 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },

		{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f },    // side 6
		{ -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
	};

	// create a vertex buffer interface called v_buffer
	_device->CreateVertexBuffer(24 * sizeof(Cube::CubeVertex),
		0,
		Cube::CUBE_FVF,
		D3DPOOL_MANAGED,
		&_vertexBuffer,
		NULL);

	// lock vertexbuffer and load vetices into it
	VOID* pVoid;
	_vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	_vertexBuffer->Unlock();

	return true;
}

bool Cube::initIndicesBuffer()
{
	// indices
	short indices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 5, 6,    // side 2
		6, 5, 7,
		8, 9, 10,    // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};

	// create an index buffer interface called i_buffer
	_device->CreateIndexBuffer(36 * sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_indicesBuffer,
		NULL);

	// lock indicesBuffer and load the indices into it
	VOID* pVoid;
	_indicesBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	_indicesBuffer->Unlock();

	return true;
}
