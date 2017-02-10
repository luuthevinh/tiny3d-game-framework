#pragma once

#include "..\TinyDefinitions.h"

NS_TINY_BEGIN

class Size
{
public:
	Size() 
	{
		width = 0.0f;
		height = 0.0f;
	}

	Size(float w, float h)
	{
		width = w;
		height = h;
	}

	float width;
	float height;
};

NS_TINY_END