#include <iostream>
#include <cstdlib>
#include <windows.h>
#include "byte_order.h"
#include "File.h"

using namespace std;

int main(int argc, char* argv[]) {

	//endianType True == SmallEndian
	//endianType False == BigEndian

	bool endianType = (su::cur_byte_order() == su::byte_order::little_endian);
	std::cout << "Your processor has a " << (endianType ? "little" : "big") << " pipi\n";
	
	//f->getFileInfo();
	try {
		const char* f1 = argv[1];

		switch (argc)
		{
		case 2: {
			File* f = new File(argv[1], endianType);
			f->readMessage();
			break;
		}
			case 3: {			
				cout << argv[1] << endl; 
				cout << argv[2] << endl;
				cout << argv[0] << endl;
				File* f = new File(argv[1], endianType, argv[2] );
				f->copyFile();
			break;
		}
		}
	}
	catch (char* err)
	{
		cout << "Err : " << err << endl;
		exit(1);
	}

	
	return 0;
}