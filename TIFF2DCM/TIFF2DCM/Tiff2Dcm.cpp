#include "Tiff2Dcm.h"


Tiff2Dcm::Tiff2Dcm()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

Tiff2Dcm::~Tiff2Dcm()
{
	delete this->bitmap;
	GdiplusShutdown(gdiplusToken);
}

void Tiff2Dcm::convertBMPtoDCM(const std::wstring path)
{
	this->convertTIFFtoBMP(path);
}

void Tiff2Dcm::convertTIFFtoBMP(const std::wstring path)
{
	std::ifstream file(path.c_str());

	if (file.good())
		this->bitmap = Bitmap::FromFile(path.c_str(), 1);

	//save to file
	//CLSID pngClsid;
	//CLSIDFromString(L"{557cf400-1a04-11d3-9a73-0000f81ef32e}", &pngClsid);
	//this->bitmap->Save(L"test.bmp", &pngClsid, NULL);	
}

