#pragma once

#include <d3dx9.h>
#include "TinyDefinitions.h"
#include "base\Object.h"
#include "base\EventKeyboard.h"

NS_TINY_BEGIN

class EventMouse;

class Camera : public Object
{
public:
	Camera(const Vec3& position, const Vec3& eye, const Vec3& up);
	~Camera();

	virtual bool init() override;
	virtual void update(float dt) override;

	void strafe(float delta);
	void fly(float delta);
	void walk(float delta);

	void pitch(float angle);
	void yaw(float angle);
	void roll(float angle);

	void setPosition(float x, float y, float z);

	const D3DXMATRIX& getViewMatrix();
	const D3DXMATRIX& getPerspectiveMatrix();

	void setFOV(float degree);
	float getFOV();

	void setAspect(float value);
	float getAspect();

	void setNearViewPlane(float value);
	float getNearViewPlane();

	void setFarViewPlane(float value);
	float getFarViewPlane();

	void onKeyPressed(EventKeyboard::KeyCode keycode);
	void onKeyReleased(EventKeyboard::KeyCode keycode);

	void onMouseMoved(EventMouse* e);
	void onMousePressed(EventMouse* e);

private:
	Vec3 _position;
	Vec3 _up;
	Vec3 _right;
	Vec3 _look;

	float _fovY;
	float _aspect;
	float _nearViewPlane;
	float _farViewPlane;

	float _sensitive;

	D3DXMATRIX _viewMatrix;
	D3DXMATRIX _projectionMatrix;

	float _walkSpeed;
	float _strafeSpeed;
	float _rollSpeed;

	bool _enableMouse;
};

NS_TINY_END