#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <filesystem>
#include <unordered_map>
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

class Tiff2Dcm
{
	private:
		std::vector<std::wstring> paths;
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		std::wstring pathData;
		std::vector<Uint8> pixelData;
		ulong nrPixels;
		std::unordered_map<std::string, std::string> tags;

	public:

		/*
			*Constructor, initializes GDI+ and cleansup residual data (just in case)
			*@return none
		*/
		Tiff2Dcm();

		/* 
			*Destructor, frees GDI+ and cleansup residual data
			*@return none
		*/
		~Tiff2Dcm();

		/*
			* Start the conversion. Creates a .dcm file from one or many .jpeg files
			*  @param pathIN - [in] The absolute path for where the .tiff file is located
			*  @param pathDATA - [in] The absolute path for where the .dat file is located
			*  @param  pathOUT - [in] The absolute path for where the .dcm file will be saved
			*  @return none
		*/
		void convertJPEGtoDCM(const std::wstring& pathIN, const std::wstring& pathDATA, const std::wstring& pathOUT);
		
	private:
		void convertTIFFtoJPEG(const std::wstring& path);
		void cleanUP();
		void extractPixelData(const std::wstring& path);
		std::string findTag(const std::string& search);
		OFCondition insertTags(DcmDataset* dcmDataSet);
		OFCondition readTags(const std::wstring& path);
		void insertFrames(DcmDataset *dcmDataSet, long& frames);
		friend std::string convertWstring(std::wstring wstr);
		friend std::vector<std::string> split(std::string str, const std::string& delim);
		friend std::wstring readFile(const std::wstring& path);
		friend size_t GetSizeOfFile(const std::wstring& path);
};