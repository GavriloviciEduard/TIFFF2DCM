//#include "vld.h"
#include "Tiff2Dcm.h"

int wmain(int argc, wchar_t *argv[])
{

	Tiff2Dcm converter = Tiff2Dcm();
	std::wstring name(argv[0]);
	converter.convertBMPtoDCM(argv[0],argv[1]);

}

	
