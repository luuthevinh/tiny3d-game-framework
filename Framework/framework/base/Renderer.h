#pragma once

#include "..\TinyDefinitions.h"
#include "Component.h"
#include "Transform.h"

NS_TINY_BEGIN

class Renderer : public Component
{
public:
	enum FillMode
	{
		POINT = 1,
		WIREFRAME = 2,
		SOLID = 3,
		FORCE = 0x7fffffff
	};

	// Inherited via Component
	virtual bool init() override;

	virtual void draw();
	virtual void updateTransform(const Transform& transform);

	void setFillMode(const FillMode& mode);
	const Renderer::FillMode& getFillMode();

protected:
	LPDIRECT3DDEVICE9 _device;

	FillMode _fillMode;
};

NS_TINY_END