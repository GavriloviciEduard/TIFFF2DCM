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
#include "dcmtk/dcmdata/dcpixseq.h"
#include "dcmtk/dcmdata/dcpxitem.h"
#include "dcmtk/dcmdata/dcpixel.h"


using namespace std::experimental::filesystem::v1;
using namespace Gdiplus;
#pragma comment (lib, "gdiplus.lib")

class Tiff2Dcm
{
	public:
	
		struct I2DImgDataInfo
		{
		/* Helper structure for storing and comparing extracted pixel data image informations */
	
		//Rows of image   
		Uint16	rows;
		//Columns of image
		Uint16	cols;
		//Number of components per pixel  
		Uint16	samplesPerPixel;
		//The DICOM color model used for the compressed data
		OFString photoMetrInt;
		//Bits Allocated for one sample
		Uint16	bitsAlloc;
		//Bits Stored, number of bits stored within Bits Allocated
		Uint16	bitsStored;
		//High Bit, hightest bit position set within Bits Allocated
		Uint16	highBit;
		//Pixel Representation (0=unsigned, 1=signed)
		Uint16	pixelRepr;
		//Planar Configuration
		Uint16	planConf;
		//Horizontal value of pixel aspect ratio
		Uint16	pixAspectH;
		//Vertical value of pixel aspect ratio
		Uint16	pixAspectV;
		
		//The transfer syntax imposed by the imported pixel pixel data.
		//This is necessary for the JPEG importer that needs to report
		//which TS must be used for the imported JPEG data (ie. baseline, progressive, ...).
		E_TransferSyntax	transSyn;
		
		/** Compares if all members of both instances are equal
		   *  @param  other - [in] comparism instance
		   *  @return True if all member are identical, false otherwise
	     */

		OFBool operator==(const I2DImgDataInfo& other)
			{
			return
				(rows == other.rows) &&
				(cols == other.cols) &&
				(samplesPerPixel == other.samplesPerPixel) &&
				(photoMetrInt == other.photoMetrInt) &&
				(bitsAlloc == other.bitsAlloc) &&
				(bitsStored == other.bitsStored) &&
				(highBit == other.highBit) &&
				(pixelRepr == other.pixelRepr) &&
				(planConf == other.planConf) &&
				(pixAspectH == other.pixAspectH) &&
				(pixAspectV == other.pixAspectV) &&
				(transSyn == other.transSyn);
			};
		
		OFBool operator!=(const I2DImgDataInfo& other)
		{
				 return !(*this == other);
		};
		
			
	};

	private:
		std::vector<std::wstring> paths;
		std::vector<std::wstring> dcms;
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		std::wstring pathData;
		std::wstring pathDCM;

	public:
		Tiff2Dcm();
		~Tiff2Dcm();
		void convertBMPtoDCM(const std::wstring path, const OFString& outputFile);

	private:
		void convertTIFFtoBMP(const std::wstring path);
		void saveDCMTemp(I2DImgSource *inputPlug,Image2Dcm& i2d , I2DOutputPlug *outPlug, E_TransferSyntax writeXfer, E_GrpLenEncoding grpLengthEnc, 
			E_EncodingType lengthEnc, E_PaddingEncoding padEnc, OFCmdUnsignedInt filepad, E_FileWriteMode writeMode, OFCmdUnsignedInt itempad);
		void mergeDCM(DcmDataset *resultObject);
		void cleanUP();
		OFCondition readAndInsertPixelData(I2DImgSource* imageSource, OFList<OFString>* imageFiles, DcmDataset * dset, E_TransferSyntax & outputTS);
};

