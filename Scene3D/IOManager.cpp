#include "stdafx.h"
#include "IOManager.h"

#include <fstream>

bool IOManager::readFileToBuffer(std::string filepath, std::vector<unsigned char>& buffer)
{
	std::ifstream file(filepath, std::ios::binary);
	if (file.fail()){
		perror(filepath.c_str());
		return false;
	}


	// seek end of file and get file size
	file.seekg(0, std::ios::end);
	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	// reduce the file size of any header present in file
	fileSize -= file.tellg();

	buffer.resize(fileSize);
	file.read((char *)&(buffer[0]), fileSize);
	file.close();
	return true;
}