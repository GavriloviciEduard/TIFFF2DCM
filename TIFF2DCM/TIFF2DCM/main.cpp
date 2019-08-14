//#include "vld.h"
#include "Tiff2Dcm.h"

int wmain(int argc, wchar_t *argv[])
{

	if (argv[0] && argv[1] && argv[2])
	{
		Tiff2Dcm converter = Tiff2Dcm();
		std::wstring name(argv[0]);
		converter.convertBMPtoDCM(argv[1], argv[2]);

		return 0;
	}

	Tiff2Dcm converter = Tiff2Dcm();
	converter.readTags(L"patient8.txt");
	//else
		//return - 1; //wrong arguments
}

	
