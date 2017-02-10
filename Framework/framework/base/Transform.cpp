#include "Transform.h"

USING_NS_TINY;

Transform::Transform() :
	position(0.0f, 0.0f, 0.0f),
	rotate(0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 1.0f)
{
}

bool Transform::init()
{
	return true;
}

const Vec3 & Transform::getPosition() const
{
	return position;
}

void Transform::setPosition(const Vec3 & pos)
{
	position = pos;
}

void Transform::setPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Transform::setPositionX(float x)
{
	position.x = x;
}

void Transform::setPositionY(float y)
{
	position.y = y;
}

void Transform::setPositionZ(float z)
{
	position.z = z;
}

const Vec3 & Transform::getScale() const
{
	return scale;
}

void Transform::setScale(const Vec3 & value)
{
	scale = value;
}

void Transform::setScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

void Transform::setScale(float value)
{
	this->setScale(value, value, value);
}

void Transform::setScaleX(float x)
{
	scale.x = x;
}

void Transform::setScaleY(float y)
{
	scale.y = y;
}

void Transform::setScaleZ(float z)
{
	scale.z = z;
}

const Vec3 & Transform::getRotate() const
{
	return rotate;
}

void Transform::setRotate(const Vec3 & r)
{
	rotate = r;
}

void Transform::setRotate(float x, float y, float z)
{
	rotate.x = x;
	rotate.y = y;
	rotate.z = z;
}

void Transform::setRotateX(float x)
{
	rotate.x = x;
}

void Transform::setRotateY(float y)
{
	rotate.y = y;
}

void Transform::setRotateZ(float z)
{
	rotate.z = z;
}