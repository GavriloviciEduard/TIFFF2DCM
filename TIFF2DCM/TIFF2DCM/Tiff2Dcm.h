#pragma once

#include <fstream>
#include <Windows.h>
#include <gdiplus.h>
#include <fstream>
#include <string>
#include <unordered_map>
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
		std::unordered_map<std::string, std::string> tags;

	public:
		Tiff2Dcm();
		~Tiff2Dcm();
		void convertJPEGtoDCM(const std::wstring& pathIN, const std::wstring& pathDATA, const std::wstring& pathOUT);
		

	private:
		void convertTIFFtoJPEG(const std::wstring path);
		void cleanUP();
		void extractPixelData(const std::wstring path);
		std::string findTag(const std::string& search);
		OFCondition insertTags(DcmDataset *dcmDataSet);
		OFCondition readTags(const std::wstring path);
		friend std::string convertWstring(std::wstring wstr);
		friend std::vector<std::string> split(std::string str, std::string delim);
		friend std::wstring readFile(const std::wstring path);
		friend size_t GetSizeOfFile(const std::wstring& path);
};