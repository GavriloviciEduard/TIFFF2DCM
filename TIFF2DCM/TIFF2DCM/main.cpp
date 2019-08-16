#include "Tiff2Dcm.h"

int wmain(int argc, wchar_t** argv)
{
	try
	{
		if (argc == 4 && (argv[1] != nullptr) && (argv[2] != nullptr) && (argv[3] != nullptr))
		{

			dcmconverter::Tiff2Dcm converter = dcmconverter::Tiff2Dcm();
			converter.convertJPEGtoDCM(argv[1], argv[2], argv[3]);

			return 0;
		}

		else
			return -1; //wrong arguments
	}

	catch (std::exception& e)
	{
		std::fstream exception("exceptions.txt");
		exception << e.what();
	}
		
	//dcmconverter::Tiff2Dcm converter = dcmconverter::Tiff2Dcm();
	//converter.convertJPEGtoDCM(L"ex_multi.tif", L"patient.dat",L"test.dcm");
	
}
