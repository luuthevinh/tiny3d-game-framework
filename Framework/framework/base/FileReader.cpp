#include "FileReader.h"

USING_NS_TINY;

FileReader::FileReader(std::string filePath)
{
	_filePath = filePath;
	_relativePath = filePath.substr(0, filePath.find_last_of('\\') + 1);
	_fullFilePath = "unknown";
}

FileReader::~FileReader()
{
}

std::string FileReader::getRelativePath()
{
	return _relativePath;
}

std::string FileReader::getFilePath()
{
	return _filePath;
}

std::string FileReader::getFullFilePath()
{
	return _fullFilePath;
}
