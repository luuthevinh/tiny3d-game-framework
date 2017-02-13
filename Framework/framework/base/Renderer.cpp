#include "Renderer.h"
#include "..\SceneManager.h"

USING_NS_TINY;

bool Renderer::init()
{
	_device = SceneManager::getInstance()->getDevice()->getDirec3DDevice();

	_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	_fillMode = Renderer::FillMode::SOLID;

	return true;
}

void Renderer::draw()
{
	if (_target != nullptr)
	{
		if (_target->getTransform() != nullptr)
		{
			this->updateTransform(*_target->getTransform());
		}
	}
}

void Renderer::updateTransform(const Transform & transform)
{
	auto position = transform.getPosition();
	auto scale = transform.getScale();
	auto rotate = transform.getRotate();

	// scale
	D3DXMATRIX scaleMat;
	D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);

	// rotate
	D3DXMATRIX rotateMatX;
	D3DXMATRIX rotateMatY;
	D3DXMATRIX rotateMatZ;
	D3DXMatrixRotationX(&rotateMatX, D3DXToRadian(rotate.x));
	D3DXMatrixRotationY(&rotateMatY, D3DXToRadian(rotate.y));
	D3DXMatrixRotationZ(&rotateMatZ, D3DXToRadian(rotate.z));

	// translate
	D3DXMATRIX translateMat;
	D3DXMatrixTranslation(&translateMat, position.x, position.y, position.z);

	// set transform
	_device->SetTransform(D3DTS_WORLD, &(rotateMatX * rotateMatY * rotateMatZ * scaleMat * translateMat));
}

void Renderer::setFillMode(const FillMode & mode)
{
	_fillMode = mode;
}

const Renderer::FillMode& Renderer::getFillMode()
{
	return _fillMode;
}

