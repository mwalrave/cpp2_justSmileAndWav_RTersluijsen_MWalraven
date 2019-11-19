#include <iostream>
#include <cstdlib>
#include <windows.h>
#include "byte_order.h"
#include "File.h"
#define _CRTDBG_MAP_ALLOC
#include "DebugNew.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

int main(int argc, char* argv[]) {

	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);
	//endianType True == SmallEndian
	//endianType False == BigEndian

	bool endianType = (su::cur_byte_order() == su::byte_order::little_endian);
	std::cerr << "Your processor has a " << (endianType ? "little" : "big") << " pipi\n";
	
	try {
		const char* f1 = argv[1];

		switch (argc)
		{
		case 2: {
			unique_ptr<File> f{ new File(argv[1], endianType) };
			
			f->readMessage();
			break;
		}
			case 3: {			
				unique_ptr<File> f{ new File(argv[1], endianType, argv[2]) };
				f->copyFile();
			break;
		}
		}
	}
	catch (char* err)
	{
		cerr << "Err : " << err << endl;
		exit(1);
	}

	
	return 0;
}