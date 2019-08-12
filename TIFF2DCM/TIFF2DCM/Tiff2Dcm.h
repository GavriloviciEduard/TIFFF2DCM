#pragma once
#include <fstream>
#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
#include <string>
#include <fstream>
using namespace Gdiplus;
#pragma comment (lib, "gdiplus.lib")

//bmp: {557cf400 - 1a04 - 11d3 - 9a73 - 0000f81ef32e}
//jpg: {557cf401 - 1a04 - 11d3 - 9a73 - 0000f81ef32e}
//gif: {557cf402 - 1a04 - 11d3 - 9a73 - 0000f81ef32e}
//tif: {557cf405 - 1a04 - 11d3 - 9a73 - 0000f81ef32e}
//png: {557cf406 - 1a04 - 11d3 - 9a73 - 0000f81ef32e}

class Tiff2Dcm
{

	private:
		Bitmap* bitmap;
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;

	public:
		Tiff2Dcm();
		~Tiff2Dcm() { GdiplusShutdown(gdiplusToken); }
		void convertBMPtoDCM(const std::wstring path);

	private:
		void convertTIFFtoBMP(const std::wstring path);
};

