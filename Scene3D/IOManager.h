#pragma once
class IOManager
{
public:
	static bool readFileToBuffer(std::string filepath, std::vector<unsigned char>& buffer);
};

