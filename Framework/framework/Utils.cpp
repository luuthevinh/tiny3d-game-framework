#include "Utils.h"
#include <sstream>
#include <assert.h>

USING_NS_TINY;

D3DMATERIAL9 Utils::WHITE_MATERIAL = {
	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  // set diffuse color to white
	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  // set ambient color to white
	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),  // set ambient color to white
	D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),	// Emissive
	2.0f								// power
};

Utils::Utils()
{
}

const std::vector<std::string> Utils::splitString(const char* source, char delim)
{
	std::stringstream ss;
	ss.str(source);

	std::string item;
	std::vector<std::string> result(0);

	while (std::getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}

std::string Utils::toString(const wchar_t * source, unsigned int codePage)
{
	assert(source != NULL);

	size_t sourceLength = std::wcslen(source);

	if (sourceLength > 0)
	{
		int length = WideCharToMultiByte(codePage, 0, source, sourceLength, NULL, 0, NULL, NULL);
		if (length == 0)
			return std::string();

		std::vector<char> buffer(length);
		WideCharToMultiByte(codePage, 0, source, sourceLength, &buffer[0], length, NULL, NULL);

		return std::string(buffer.begin(), buffer.end());
	}
		
	return std::string();
}
