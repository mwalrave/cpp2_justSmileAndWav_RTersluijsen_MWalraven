#include "File.h"
#include <fstream>
#include <bitset>
using namespace std;



File::File()
{
}


File::File(string pFileName, bool pLittleEndian) : fileName(pFileName), littleEndian(pLittleEndian)
{
	setFileInfo();

}

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
		int size = 0;
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

				break;
			}
			else {

				ifs.seekg(size, ifs.cur);
			}

		}

		cout << buffer.data() << " Data" << endl;
		cout << size << " Data size" << endl;
		cout << bitsPerSample << " Bits per sample" << endl;
		int numberOfSamples = size / 2;
		cout << numberOfSamples << " numberOfSamples " << endl;

		std::vector<short> storage(size);
		for (int i = 0; i < numberOfSamples; i++) {

			ifs.read((char*)& storage[i], 2);


		}


		std::vector<bitset<8>> hiddenMessageBytes(numberOfSamples);
		std::bitset<8> hiddenCharacterByte;
		int j = 7;

		for (int i = 0; i < numberOfSamples; i++) {

			
			int lsb = storage[i] & 1;
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
		}cout << " "<< endl;
		ifs.close();


	/*	std::string message = "";
		for (int i = 0; i < hiddenMessageBytes.size(); i++) {
			auto characterByte = hiddenMessageBytes.at(i);
				cout << characterByte << endl;
		}*/

		//cout << endl << message << endl;

	}


}

void File::getFileInfo()
{

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



