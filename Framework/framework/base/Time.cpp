#include "Time.h"

NS_TINY_BEGIN

Time::Time()
{
}

Time::~Time()
{
}

void Time::init()
{
	_totalTime = (float)clock() / CLOCKS_PER_SEC;
	_elapsedTime = 0.0f;
}

void Time::update()
{
	// thời gian hiện tại
	float curTime = (float)clock() / CLOCKS_PER_SEC;

	// thời gian trôi qua từ lần update trước
	_elapsedTime = curTime - _totalTime;

	// tổng thời gian = thời gian hiện tại
	_totalTime = curTime;
}

float Time::getElapsedTime() const
{
	return _elapsedTime;
}

float Time::getTotalTime() const
{
	return _totalTime;
}

clock_t Time::getTicks() const
{
	return clock();
}

NS_TINY_END