#include "File.h"
#include <fstream>
#include <bitset>
using namespace std;



File::File() 
{
}


File::File(string pFileName, bool pLittleEndian) : fileName(pFileName), littleEndian(pLittleEndian)
{
	if (!fileExists(fileName)) {
		cout << "!File doesnt exist, shame on you!" << endl;
		exit(0);
	};
	setFileInfo();

}

File::File(string pFileName, bool pLittleEndian,  std::string pMessage): File(pFileName, pLittleEndian)
{
	message = pMessage;
	cout << "Imprint Message: " << message << endl;
	

}

bool File::fileExists(const std::string& fileName) {
	ifstream f(fileName.c_str());

	bool exists = f.good();
	f.close();

	return exists;
}

// Misschien wat opdelen?
void File::setFileInfo()
{

	FILE* wfile;
	std::ifstream ifs(fileName, std::ifstream::binary);

	if (ifs) {
		ifs.seekg(8, ifs.beg);

		std::vector<char> buffer(5);


		ifs.read(buffer.data(), 4);

		extension = buffer.data();
		if (extension == "WAVE") {
			cout << "File accepted, very wave" << endl;
		}
		else {
			cout << "File is pretty wrong, this is no wave??? " << endl;

			ifs.close();
			exit(1);
		}

		std::vector<char> chunkSize(5);
		size = 0;
		while (true) {
			ifs.read(buffer.data(), 4);
			ifs.read(chunkSize.data(), 4);




			size = toInt(chunkSize);
			if (strstr(buffer.data(), "fmt")) {
				std::vector<char> sampleSize(5);
				ifs.seekg(size - 2, ifs.cur);
				ifs.read(sampleSize.data(), 2);
				bitsPerSample = toInt(sampleSize);
			}

			else if (strstr(buffer.data(), "data")) {
				dataPosition = ifs.tellg();
				break;
			}
			else {

				ifs.seekg(size, ifs.cur);
			}

		}

		ifs.close();

		bytesPerSample = bitsPerSample / 8;
		numberOfSamples = size / bytesPerSample;
		
		cout << buffer.data() << " Data" << endl;
		cout << size << " Data size" << endl;
		cout << bitsPerSample << " Bits per sample" << endl;
		
		cout << numberOfSamples << " numberOfSamples " << endl << endl;
		

	}


}

void File::getFileInfo()
{

}

void File::readMessage()
{

	std::ifstream ifs(fileName, std::ifstream::binary);
	ifs.seekg(dataPosition);
	std::vector<short> storage(size);
	for (int i = 0; i < numberOfSamples; i++) {

		ifs.read((char*)& storage[i], bytesPerSample);


	}


	std::vector<bitset<8>> hiddenMessageBytes(numberOfSamples);
	std::bitset<8> hiddenCharacterByte;
	int j = 7;

	for (int i = 0; i < numberOfSamples; i++) {


		int lsb = storage[i] & 1;
		//cout << storage[i] << endl;
		hiddenCharacterByte.set(j, lsb);

		//cout << lsb << "";
		if (j > 0) {
			j--;
		}
		else {
			j = 7;
			if (hiddenCharacterByte.none()) {
				break;
			};
			hiddenMessageBytes[i] = hiddenCharacterByte;
			cout << (char)hiddenCharacterByte.to_ulong();

		}
	}
	cout << " " << endl;
	ifs.close();
}

void File::copyFile()
{
		string newFileName = "output" + fileName;
		int messageLength = message.length();

		std::ifstream  src(fileName, std::ios::binary);

		src.seekg(0, src.end);
		long size = src.tellg();
		src.seekg(0, src.beg);


		std::vector<char> output(size);

		std::vector<short> storage(1 + messageLength * 8);

		src.read(output.data(), dataPosition);

		for (int i = 0; i < messageLength * 8; i++) {
			src.read((char*)& storage[i], 2);
		}
	

		std::vector<bitset<8>> messageBitsets(messageLength);

		for (int i = 0; i < messageLength; i++) {
			messageBitsets[i] = bitset<8>(message.c_str()[i]);
			cout << messageBitsets[i] << " ";
		}
		cout << endl;
		int counter = 0;
		for (int i = 0; i < messageLength; i++) {
			for (int j = 7; j >= 0; j--) {
				int lsb = storage[counter] & 1;
				if (lsb != messageBitsets[i][j]) {

					storage[counter] ^= 1 << 0;

				}



				counter++;

			}
		
		}	


		long streamPos = dataPosition;
		for (int i = 0; i < messageLength * 8; i++) {
			cout << (char)storage[i] << " Char " << endl;
			//short value = storage[i];

			//output[streamPos + 1] = value;
			//value = value >> 8;
			//output[streamPos ] = value;
			//streamPos += 2;

			memcpy(&output[streamPos], &storage[i], 2);
		}
		streamPos = src.tellg();
		vector<char> newBuffer(size - streamPos);
		src.read(newBuffer.data(), size);


		cout << dataPosition << endl;

		cout << streamPos << endl;
		for (int i = 0; i < size - streamPos; i++) {
			output.at((i + streamPos)) = newBuffer[i];
		}


		std::ofstream  dst(newFileName, std::ios::binary);
		dst.write(output.data(), size);

		//dst << src.rdbuf();
		src.close();
		dst.close();

}

int File::toInt(std::vector<char> chunkSize)
{
	int size = 0;
	for (int n = sizeof(size); n >= 0; n--) {
		size = (size << 8) + chunkSize[n];
	}


	if (littleEndian)
		for (int n = sizeof(size); n >= 0; n--)
			size = (size << 8) + chunkSize[n];
	else
		for (unsigned n = 0; n < sizeof(size); n++)
			size = (size << 8) + chunkSize[n];
	return size;
}

void File::setMessage(std::string message)
{
}



