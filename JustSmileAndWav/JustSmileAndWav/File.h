#pragma once
#include <string>
#include <iostream>
#include <vector>

class File {
public:
	File();
	File(std::string pFileName, bool pLittleEndian);
	File(std::string pFileName, bool pLittleEndian, std::string message);
	bool fileExists(const std::string& fileName);
	void setFileInfo();
	void getFileInfo();
	void readMessage();
	void copyFile();
	int toInt(std::vector<char> chunkSize);

	//Vars

	std::string message;
	bool littleEndian;
	std::string fileName;
	std::string extension;
	int bitsPerSample;
	int bytesPerSample;
	int numberOfSamples;
	int dataPosition;
	int size;
	void setMessage(std::string message);
};
