#include "vld.h"
#include "Tiff2Dcm.h"

int main()
{

	Tiff2Dcm converter = Tiff2Dcm();
	std::wstring name(L"ex_multi.tif");
	const wchar_t* path = name.c_str();
	converter.convertBMPtoDCM(path, "caca.dcm");

;
}

	
