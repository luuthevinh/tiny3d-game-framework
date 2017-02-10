#pragma once

#include <ctime>
#include "..\TinyDefinitions.h"

NS_TINY_BEGIN

class Time
{
public:
	Time();
	~Time();

	virtual void init();
	virtual void update();

	// Tổng thời gian trôi qua kể từ lần cuối gọi update
	float getElapsedTime() const;

	// Tổng thời gian trôi qua kể từ lúc gọi init
	float getTotalTime() const;

	clock_t getTicks() const;

private:
	float _elapsedTime;
	float _totalTime;

	clock_t _totalTicks;

};

NS_TINY_END