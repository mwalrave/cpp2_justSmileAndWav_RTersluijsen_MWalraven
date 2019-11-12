#pragma once
#include <string>
#include <iostream>
#include <vector>

class File {
public:
	File();
	File(std::string pFileName, bool pLittleEndian);
	void setFileInfo();
	void getFileInfo();
	int toInt(std::vector<char> chunkSize);

	//Vars
	bool littleEndian;
	std::string fileName;
	std::string extension;
	int bitsPerSample;
};
