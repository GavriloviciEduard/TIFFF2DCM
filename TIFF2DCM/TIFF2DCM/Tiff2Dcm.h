#pragma once

#include <fstream>
#include <Windows.h>
#include <gdiplus.h>
#include <fstream>
#include <string>
#include <experimental/filesystem> 
#include <filesystem>
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdict.h"
#include "dcmtk/dcmdata/libi2d/i2djpgs.h"
#include "dcmtk/dcmdata/libi2d/i2d.h"
#include "dcmtk/dcmdata/libi2d/i2dplnsc.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcpixseq.h"
#include "dcmtk/dcmdata/dcpxitem.h"
#include "dcmtk/dcmjpeg/djencode.h"


using namespace std::experimental::filesystem::v1;
using namespace Gdiplus;
#pragma comment (lib, "gdiplus.lib")

class Tiff2Dcm
{
	private:
		std::vector<std::wstring> paths;
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		std::wstring pathData;
		std::vector<Uint8> pixeldata;
		ulong nrpixels;
		std::vector<std::string> tagstxt;
		

	public:
		Tiff2Dcm();
		~Tiff2Dcm();
		void convertBMPtoDCM(const std::wstring pathIN, const std::wstring pathOUT);
		void readTags(const std::wstring path);

	private:
		void convertTIFFtoJPEG(const std::wstring path);
		void cleanUP();
		void extractPixelData(const std::wstring path);
		OFCondition CreateHeaderImage(DcmDataset *dcmDataSet);
		std::string convertWstring(std::wstring wstr);
		std::vector<std::string> split(std::string str, std::string delim);


};