#include "File.h"
#include <fstream>
using namespace std;



File::File()
{
}


File::File(string pFileName, bool pLittleEndian) : fileName(pFileName), littleEndian( pLittleEndian)
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
			
			else if (strstr(buffer.data(),"data")) {
			
				break;
			}
			else {

				ifs.seekg(size, ifs.cur);
			}
			
		}

		cout << buffer.data() << " Data" << endl;
		cout << size << " Data size" <<endl;
		cout << bitsPerSample << " Bits per sample"<< endl;
		int numberOfSamples = size / bitsPerSample;
		cout << numberOfSamples << " NUMBAH " << endl;
		std::vector< std::vector<char> > samples(numberOfSamples, vector<char>(bitsPerSample+4));
		for(int i = 0; i < numberOfSamples; i++){
			ifs.read(samples[i].data() , bitsPerSample);
			cout << samples[i].data() << endl;
		}
		ifs.close();

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



