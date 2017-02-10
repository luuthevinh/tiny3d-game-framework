#pragma once

#include "..\TinyDefinitions.h"
#include "Component.h"

NS_TINY_BEGIN

class Transform : public Component
{
public:
	Vec3 position;
	Vec3 scale;

	// degree
	Vec3 rotate;

	Transform();
	// Inherited via Component
	virtual bool init() override;

	virtual const Vec3& getPosition() const;
	virtual void setPosition(const Vec3& position);
	virtual void setPosition(float x, float y, float z);
	virtual void setPositionX(float x);
	virtual void setPositionY(float y);
	virtual void setPositionZ(float z);

	virtual const Vec3& getScale() const;
	virtual void setScale(const Vec3& value);
	virtual void setScale(float x, float y, float z);
	virtual void setScale(float value);
	virtual void setScaleX(float x);
	virtual void setScaleY(float y);
	virtual void setScaleZ(float z);

	virtual const Vec3& getRotate() const;
	virtual void setRotate(const Vec3& rotate);
	virtual void setRotate(float x, float y, float z);
	virtual void setRotateX(float x);
	virtual void setRotateY(float y);
	virtual void setRotateZ(float z);

private:
};

NS_TINY_END