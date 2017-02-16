#include "Mesh.h"
#include "Transform.h"
#include "..\renderer\MeshRenderer.h"
#include "..\SceneManager.h"

#include "MeshLoader.h"

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

	// load from .obj file
	if (!MeshLoader::loadFromOBJFile(this, filePath, _device))
	{
		return false;
	}

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

void Mesh::addMaterial(D3DMATERIAL9 material)
{
	_renderer->addMaterial(material);
}

void Mesh::addTexture(LPDIRECT3DTEXTURE9 texture)
{
	_renderer->addTexture(texture);
}
