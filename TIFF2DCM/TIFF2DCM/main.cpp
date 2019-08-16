#include "Tiff2Dcm.h"

int wmain(int argc, wchar_t *argv[])
{

	if (argc == 4 && argv[1] && argv[2] && argv[3])
	{
		Tiff2Dcm converter = Tiff2Dcm();
		converter.convertJPEGtoDCM(argv[1], argv[2], argv[3]);


		return 0;
	}

	else
		return - 1; //wrong arguments
		
	//Tiff2Dcm converter = Tiff2Dcm();
	//converter.convertJPEGtoDCM(L"ex_multi.tif", L"patient.dat",L"test.dcm");
	
}
