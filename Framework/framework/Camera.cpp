#include "Camera.h"
#include "..\Settings.h"
#include "SceneManager.h"
#include "base\EventKeyboardListener.h"
#include "base\EventDispatcher.h"
#include "base\EventMouseListener.h"
#include "base\EventMouse.h"

USING_NS_TINY;

Camera::Camera(const Vec3 & position, const Vec3 & eye, const Vec3 & up)
{
	_position = position;
	_look = eye - position;
	_up = up;

	_fovY = 45.0f;
	_aspect = (float)Settings::getInstance()->getWindowSize().width / Settings::getInstance()->getWindowSize().height;
	_nearViewPlane = 1.0f;
	_farViewPlane = 1000.0f;

	_sensitive = 0.001f;
	_enableMouse = false;
}

Camera::~Camera()
{
}

bool Camera::init()
{
	auto dispatcher = SceneManager::getInstance()->getEventDispatcher();

	auto listener = new EventKeyboardListener();
	listener->onKeyPressed = CALLBACK_01(&Camera::onKeyPressed, this);
	listener->onKeyReleased = CALLBACK_01(&Camera::onKeyReleased, this);

	dispatcher->addEventListener(listener, this);

	auto mouse = new EventMouseListener();
	mouse->onMouseMoved = CALLBACK_01(&Camera::onMouseMoved, this);

	dispatcher->addEventListener(mouse, this);

	return true;
}

void Camera::update(float dt)
{
	this->walk(_walkSpeed * dt);
	this->strafe(_strafeSpeed * dt);

	auto device = SceneManager::getInstance()->getDevice()->getDirec3DDevice();

	D3DXMATRIX viewMat;
	viewMat = this->getViewMatrix();
	device->SetTransform(D3DTS_VIEW, &viewMat);

	D3DXMATRIX matProjection;
	matProjection = this->getPerspectiveMatrix();
	device->SetTransform(D3DTS_PROJECTION, &matProjection);
}

void Camera::strafe(float delta)
{
	_position += _right * delta;
}

void Camera::fly(float delta)
{
	_position += _up * delta;
}

void Camera::walk(float delta)
{
	_position += _look * delta;
}

void Camera::pitch(float angle)
{
	D3DXMATRIX mat;
	D3DXMatrixRotationAxis(&mat, &_right, angle);

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&_up, &_up, &mat);
	D3DXVec3TransformCoord(&_look, &_look, &mat);
}

void Camera::yaw(float angle)
{
	D3DXMATRIX mat;
	D3DXMatrixRotationAxis(&mat, &_up, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&_right, &_right, &mat);
	D3DXVec3TransformCoord(&_look, &_look, &mat);
}

void Camera::roll(float angle)
{
	D3DXMATRIX mat;
	D3DXMatrixRotationAxis(&mat, &_look, angle);

	// rotate _up and _right around _look vector
	D3DXVec3TransformCoord(&_right, &_right, &mat);
	D3DXVec3TransformCoord(&_up, &_up, &mat);
}

void Camera::setPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
}

const D3DXMATRIX& Camera::getViewMatrix()
{
	// chuẩn hóa vector hướng của camera
	D3DXVec3Normalize(&_look, &_look);

	// vector right là pháp tuyến của look và up
	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);

	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);

	float x = -D3DXVec3Dot(&_right, &_position);
	float y = -D3DXVec3Dot(&_up, &_position);
	float z = -D3DXVec3Dot(&_look, &_position);

	(_viewMatrix)(0, 0) = _right.x;
	(_viewMatrix)(0, 1) = _up.x;
	(_viewMatrix)(0, 2) = _look.x;
	(_viewMatrix)(0, 3) = 0.0f;

	(_viewMatrix)(1, 0) = _right.y;
	(_viewMatrix)(1, 1) = _up.y;
	(_viewMatrix)(1, 2) = _look.y;
	(_viewMatrix)(1, 3) = 0.0f;

	(_viewMatrix)(2, 0) = _right.z;
	(_viewMatrix)(2, 1) = _up.z;
	(_viewMatrix)(2, 2) = _look.z;
	(_viewMatrix)(2, 3) = 0.0f;

	(_viewMatrix)(3, 0) = x;
	(_viewMatrix)(3, 1) = y;
	(_viewMatrix)(3, 2) = z;
	(_viewMatrix)(3, 3) = 1.0f;

	return _viewMatrix;
}

const D3DXMATRIX& Camera::getPerspectiveMatrix()
{
	D3DXMatrixPerspectiveFovLH(
		&_projectionMatrix,
		D3DXToRadian(_fovY),
		_aspect,
		_nearViewPlane,
		_farViewPlane);

	return _projectionMatrix;
}

void Camera::setFOV(float degree)
{
	_fovY = degree;
}

float Camera::getFOV()
{
	return _fovY;
}

void Camera::setAspect(float value)
{
	_aspect = value;
}

float Camera::getAspect()
{
	return _aspect;
}

void Camera::setNearViewPlane(float value)
{
	_nearViewPlane = value;
}

float Camera::getNearViewPlane()
{
	return _nearViewPlane;
}

void Camera::setFarViewPlane(float value)
{
	_farViewPlane = value;
}

float Camera::getFarViewPlane()
{
	return _farViewPlane;
}

void Camera::onKeyPressed(EventKeyboard::KeyCode keycode)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_W:
	{
		_walkSpeed = 20.0f;
		break;
	}
	case EventKeyboard::KeyCode::KEY_S:
	{
		_walkSpeed = -20.0f;
		break;
	}
	case EventKeyboard::KeyCode::KEY_A:
	{
		_strafeSpeed = -20.0f;
		break;
	}
	case EventKeyboard::KeyCode::KEY_D:
	{
		_strafeSpeed = 20.0f;
		break;
	}
	default:
		break;
	}
}

void Camera::onKeyReleased(EventKeyboard::KeyCode keycode)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_S:
	{
		_walkSpeed = 0.0f;
		break;
	}
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_D:
	{
		_strafeSpeed = 0.0f;
		break;
	}
	default:
		break;
	}
}

void Camera::onMouseMoved(EventMouse * e)
{
	char str[128];
	sprintf(str, "mouse: (%.2f, %.2f), delta: (%.2f, %.2f)\n", e->getPosition().x, e->getPosition().y, e->getDeltaChange().x, e->getDeltaChange().y);
	OutputDebugString(str);

	if (e->getButton() == EventMouse::Button::BUTTON_LEFT)
	{
		_enableMouse = true;

		SetCursor(NULL);
		SceneManager::getInstance()->getDevice()->getDirec3DDevice()->ShowCursor(false);
	}
	else
	{
		_enableMouse = false;
		SceneManager::getInstance()->getDevice()->getDirec3DDevice()->ShowCursor(true);
	}

	if (_enableMouse)
	{
		this->yaw(e->getDeltaChange().x * _sensitive);
		this->pitch(e->getDeltaChange().y * _sensitive);
	}
}

void Camera::onMousePressed(EventMouse * e)
{
	
}