#pragma once

#include "Event.h"

NS_TINY_BEGIN

class EventMouse : public Event
{
public:
	enum Button
	{
		BUTTON_NONE = 0,
		BUTTON_LEFT = 1,
		BUTTON_RIGHT = 2
	};

	enum EventMouseType
	{
		BUTTON_PRESSED,
		MOUSE_MOVING
	};

	EventMouse(Button button);
	EventMouse(float x, float y);

	~EventMouse();

	const Button& getButton() const;
	void setButton(const Button& button);

	void setPressed(bool pressed);
	bool isPressed();

	void setDeltaChange(float x, float y);
	const Vec2& getDeltaChange() const;

	void setPosition(const Vec2& position);
	const Vec2& getPosition() const;
private:
	Vec2 _deltaChange;
	Vec2 _position;

	Button _button;
	bool _isPressed;
};

NS_TINY_END