#pragma once

#include <d3dx9.h>
#include <vector>
#include "TinyDefinitions.h"

NS_TINY_BEGIN

class Utils
{
public:
	static D3DMATERIAL9 WHITE_MATERIAL;

	static const std::vector<std::string> splitString(const char* source, char delim);
	
	static std::string toString(const wchar_t* source, unsigned int codePage = CP_ACP);

private:
	Utils();
};

NS_TINY_END