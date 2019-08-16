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

namespace dcmconverter { class Tiff2Dcm; }
class dcmconverter::Tiff2Dcm
{
	private:
		//A vector in which the absolute paths of the .jpeg file are saved
		std::vector<std::wstring> paths;

		//GDI+
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;

		//The absolute path of the temps directory
		std::wstring pathData;

		//A vector in which the pixel data of each frame will be extracted
		std::vector<Uint8> pixelData;

		//An unsorted map in which the values of the .dat file will be save as a pair (key-value)
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
			*  Start the conversion. Creates a .dcm file from one or many .jpeg files
			*  @param pathIN - [in] The absolute path for where the .tiff file is located
			*  @param pathDATA - [in] The absolute path for where the .dat file is located
			*  @param  pathOUT - [in] The absolute path for where the .dcm file will be saved
			*  @return none
			*  @exit code(2) - could not create dcm
		*/
		void convertJPEGtoDCM(const std::wstring& pathIN, const std::wstring& pathDATA, const std::wstring& pathOUT);
		
	private:
		/*
			*  Converts a .tiff/tif file to a .jpeg file(works for multi-frame tiffs)
			*  @param path - [in] The absolute path for where the .tiff file is located
			*  @return none
			*  @exit code(1) - file not found
		*/
		void convertTIFFtoJPEG(const std::wstring& path);

		/*
			*  Clenasup residual data (jpeg created for the dcm)
			*  @params none
			*  @return none
		*/
		void cleanUP();

		/*
			*  Extracts the pixel data of in image into a buffer -> std::vector<Uint8> pixelData;
			*  @param path - [in] The absolute path for where the iamge file is located
			*  @return none
		*/
		void extractPixelData(const std::wstring& path);

		/*
			*  Search and returns a value from a given tag sorted in map -> std::unordered_map<std::string, std::string> tags;
			*  @param search - [in] The name of the value to be searched
			*  @return std::string - value of a given tag
		*/
		std::string findTag(const std::string& search);

		/*
			*  Insert a given tag with a given value in the DataSet of the .dcm file that is being constructed
			*  @param dcmDataSet - [in] The DataSet in which the tags with the given values will be inserted
			*  @return OFCondition - good() -> true if insertion was successful, bad() -> true if insertion failed
		*/
		OFCondition insertTags(DcmDataset* dcmDataSet);

		/*
			*  Extract the tags and values from a given .dat file that will be inserted in the new .dcm file
			*  @param path - [in] The absolute path for where the .dat file is located
			*  @return OFCondition - good() -> true if the extraction was successful, bad() -> true if extraction failed
			*  @exit code(3) - could not read .dat file
		*/
		void readTags(const std::wstring& path);

		/*
			*  If the .tiff file is multi-frame then the .dcm will be constructed from multiple frames
			*  The method inserts all the frames in DataSet of the new .dcm file
			*  @param dcmDataSet - [in] The DataSet in which the frames will be inserted
			*  @param frame - [in] The number of the frame that will be inserted
			*  @return none
			*  @exit code(4) - failed to insert frame
		*/
		void insertFrames(DcmDataset *dcmDataSet, long& frame);

		/*
			*  Friend method for converting std::wstring to std::string
			*  @param wstr - [in] The string that will be converted
			*  @return std::string - The converted std::string
		*/
		friend std::string convertWstring(std::wstring wstr);

		/*
			*  Friend method for spliting the data from the .dat file by a given delimitator (int our case '=')
			*  @param str - [in] The string that will be splitted
			*  @param delim - [in] The string that is the delimitator
			*  @return std::vector<std::string> - A vector of the splitted strings
		*/
		friend std::vector<std::string> split(std::string str, const std::string& delim);

		/*
			*  Friend method for reading the give .dat file (utf-8)
			*  @param path - [in] The absolute path of the .dat file
			*  @return std::wstring - The file saved is a std::wstring
		*/
		friend std::wstring readFile(const std::wstring& path);

		/*
			*  Friend method that returns the size of the .dat file (utf-8)
			*  @param path - [in] The absolute path of the .dat file
			*  @return size_t - The size of the .dat file (utf-8)
		*/
		friend size_t GetSizeOfFile(const std::wstring& path);
};

