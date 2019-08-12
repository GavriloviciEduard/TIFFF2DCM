#include "Tiff2Dcm.h"





int main()
{

	Tiff2Dcm converter = Tiff2Dcm();
    std::wstring name(L"example.tiff");
	const wchar_t* path = name.c_str();
	converter.convertBMPtoDCM(path);

    
}
