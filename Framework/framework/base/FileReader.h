#pragma once

#include <string>
#include "..\TinyDefinitions.h"

NS_TINY_BEGIN

class FileReader
{
public:
	FileReader(std::string filePath);
	~FileReader();

	virtual std::string getRelativePath();
	virtual std::string getFilePath();
	virtual std::string getFullFilePath();

protected:
	std::string _relativePath;
	std::string _filePath;
	std::string _fullFilePath;
};

NS_TINY_END