#pragma once



#include <fstream>
#include <vector>
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <fstream>
#include <experimental/filesystem> 
#include <filesystem>
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/cmdlnarg.h"
#include "dcmtk/ofstd/ofconapp.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdict.h"
#include "dcmtk/dcmdata/libi2d/i2d.h"
#include "dcmtk/dcmdata/libi2d/i2djpgs.h"
#include "dcmtk/dcmdata/libi2d/i2dbmps.h"
#include "dcmtk/dcmdata/libi2d/i2dplsc.h"
#include "dcmtk/dcmdata/libi2d/i2dplvlp.h"
#include "dcmtk/dcmdata/libi2d/i2dplnsc.h"
#include "dcmtk/dcmdata/dcdeftag.h"


using namespace std::experimental::filesystem::v1;
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
		std::vector<Bitmap*> bitmaps;
		std::vector<std::wstring> paths;
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		std::wstring pathData;
		std::wstring BMPData;

	public:
		Tiff2Dcm();
		~Tiff2Dcm();
		void convertBMPtoDCM(const std::wstring path, const OFString& outputFile);

	private:
		void convertTIFFtoBMP(const std::wstring path);
		void saveTempBMP();

};

