#include "Tiff2Dcm.h"


Tiff2Dcm::Tiff2Dcm()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); //setup GDI+
	this->pathData = L"data"; //path for tmp
	this->pathDCM = L"dcm";
	this->cleanUP(); //cleanup just in case
}

Tiff2Dcm::~Tiff2Dcm()
{
	GdiplusShutdown(gdiplusToken); //close GDI+
	this->cleanUP(); //cleanup temp
}



void Tiff2Dcm::convertBMPtoDCM(const std::wstring path, const OFString& outputFile)
{
	this->convertTIFFtoBMP(path);

	if (this->paths.size()) // if converted with succes
	{
		Image2Dcm i2d;
		E_GrpLenEncoding grpLengthEnc = EGL_recalcGL;
		E_EncodingType lengthEnc = EET_ExplicitLength;
		E_PaddingEncoding padEnc = EPD_noChange;
		OFCmdUnsignedInt filepad = 0;
		OFCmdUnsignedInt itempad = 0;
		E_FileWriteMode writeMode = EWM_fileformat;
		OFList<OFString> overrideKeys;
		E_TransferSyntax writeXfer;
		OFList<OFString> inputFiles;
		OFString  scanPattern = "";
		I2DOutputPlug *outPlug = NULL;
		I2DImgSource *inputPlug = NULL;


		outPlug = new I2DOutputPlugNewSC();
		inputPlug = new I2DBmpSource();
		DcmDataset *resultObject = new DcmDataset();
		OFBool scanDir = OFTrue;
		OFBool recurse = OFTrue;
		writeMode = EWM_fileformat;
		grpLengthEnc = EGL_recalcGL;
		lengthEnc = EET_ExplicitLength;
		i2d.setOverrideKeys(overrideKeys);
		OFBool insertLatin1 = OFTrue;
		OFBool insertType2 = OFTrue;
		OFBool doChecks = OFTrue;
		i2d.setValidityChecking(doChecks, insertType2, true);
		outPlug->setValidityChecking(doChecks, insertType2, true);
		


		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		std::string converted_str = converter.to_bytes(this->pathData);

		
		//validate directory for beeing directory or file
		if (OFStandard::dirExists(converted_str.c_str()))
		{
				OFStandard::searchDirectoryRecursively(converted_str.c_str(), inputFiles, scanPattern, "", recurse);
		}

		inputPlug->setImageFile(converted_str.c_str());
		//OFCondition cond = this->readAndInsertPixelData(inputPlug, &inputFiles, resultObject, writeXfer);




		//
		////// Main class for controlling conversion
		//Image2Dcm i2d;
		////// Output plugin to use (i.e. SOP class to write)
		//I2DOutputPlug *outPlug = NULL;
		////// Input plugin to use (i.e. file format to read)
		//I2DImgSource *inputPlug = NULL;
		////// Group length encoding mode for output DICOM file
		//E_GrpLenEncoding grpLengthEnc = EGL_recalcGL;
		////// Item and Sequence encoding mode for output DICOM file
		//E_EncodingType lengthEnc = EET_ExplicitLength;
		////// Padding mode for output DICOM file
		//E_PaddingEncoding padEnc = EPD_noChange;
		////// File pad length for output DICOM file
		//OFCmdUnsignedInt filepad = 0;
		////// Item pad length for output DICOM file
		//OFCmdUnsignedInt itempad = 0;
		////// Write only pure dataset, i.e. without meta header
		//E_FileWriteMode writeMode = EWM_fileformat;
		////// Override keys are applied at the very end of the conversion "pipeline"
		//OFList<OFString> overrideKeys;
		////// The transfer syntax proposed to be written by output plugin
		//E_TransferSyntax writeXfer;
		//DcmFileFormat file;


		//inputPlug = new I2DBmpSource();
		//outPlug = new I2DOutputPlugSC();
		//writeMode = EWM_fileformat;
		//grpLengthEnc = EGL_recalcGL;
		//lengthEnc = EET_ExplicitLength;

		//i2d.setOverrideKeys(overrideKeys);

		////// Test for ISO Latin 1 option
		//OFBool insertLatin1 = OFTrue;

		////// evaluate validity checking options
		//OFBool insertType2 = OFTrue;
		//OFBool doChecks = OFTrue;
		//i2d.setValidityChecking(doChecks, insertType2, true);
		//outPlug->setValidityChecking(doChecks, insertType2, true);


		//using convert_type = std::codecvt_utf8<wchar_t>;
		//std::wstring_convert<convert_type, wchar_t> converter;
		//std::string converted_str = converter.to_bytes(this->paths[0]);
		//inputPlug->setImageFile(converted_str.c_str());

		/////* make sure data dictionary is loaded */
		//if (!dcmDataDict.isDictionaryLoaded())
		//{
		//	return;
		//}

		//DcmDataset *resultObject = NULL;
		//OFCondition cond = i2d.convert(inputPlug, outPlug, resultObject, writeXfer);//create base file first
		////this->saveDCMTemp(inputPlug,i2d, outPlug, writeXfer,grpLengthEnc,lengthEnc,padEnc,filepad,writeMode,itempad);
		////this->mergeDCM(resultObject);

		//// Save
		//if (cond.good())
		//{

		//	DcmFileFormat dcmff(resultObject);
		//	dcmff.saveFile(outputFile.c_str(), writeXfer, lengthEnc, grpLengthEnc, padEnc, OFstatic_cast(Uint32, filepad), OFstatic_cast(Uint32, itempad), writeMode);
		//}

		//
		//delete outPlug;
		//outPlug = NULL;
		//delete inputPlug;
		//inputPlug = NULL;
		//delete resultObject;
		//resultObject = NULL;		
	}

}

void Tiff2Dcm::convertTIFFtoBMP(const std::wstring path)
{
	
	std::ifstream file(path.c_str()); //file
	std::string str(path.begin(),path.end()); //string to check extension

	if (!exists(this->pathData)) // create directory for tmps if it does not exist
		create_directory(this->pathData);

	if (file.good() && (str.substr(str.find_last_of(".") + 1) == "tiff" || str.substr(str.find_last_of(".") + 1) == "tif") && exists(this->pathData))
	{
		Bitmap* temp = Bitmap::FromFile(path.c_str(), 1); //read tiff in bitmap
		int count = temp->GetFrameCount(&FrameDimensionPage); // get frames of tiff

		for (int i = 0; i < count; i++) // for all frames in tiff
		{

			temp->SelectActiveFrame(&FrameDimensionPage, i); // current frame

			std::wstring name(L"tmp");
			std::wstring type(L".bmp");
			std::wstring nr = std::to_wstring(i);
			std::wstring fin = this->pathData + L"//" + name + nr + type;  // path to save bmp

			CLSID bmpClsid;
			CLSIDFromString(L"{557cf400-1a04-11d3-9a73-0000f81ef32e}", &bmpClsid);
			temp->Save(fin.c_str(), &bmpClsid, NULL); //save current frame as bmp 

			std::experimental::filesystem::v1::path abs = absolute(fin);
			this->paths.push_back(abs.generic_wstring()); // all absolute paths for frames(tmps)
		}


		//cleanup
		delete temp;
		file.close();
	}	
}

void Tiff2Dcm::saveDCMTemp(I2DImgSource *inputPlug, Image2Dcm& i2d, I2DOutputPlug *outPlug, E_TransferSyntax writeXfer, E_GrpLenEncoding grpLengthEnc,
E_EncodingType lengthEnc, E_PaddingEncoding padEnc, OFCmdUnsignedInt filepad, E_FileWriteMode writeMode, OFCmdUnsignedInt itempad)
{
	if (!exists(this->pathDCM)) // create directory for tmps if it does not exist
		create_directory(this->pathDCM);

	for (int i = 0; i< this->paths.size(); i++)
	{
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		std::string converted_str = converter.to_bytes(this->paths[i]);
		inputPlug->setImageFile(converted_str.c_str());

		DcmDataset *temp = NULL;
		OFCondition cond = i2d.convert(inputPlug, outPlug, temp, writeXfer);//create temp

		if (cond.good())
		{
			std::wstring type(L".dcm");
			std::wstring name(L"tmp");
			std::wstring nr = std::to_wstring(i);
			std::wstring fin = this->pathDCM + L"//" + name + nr + type;  // path to save dcm
			using convert_type = std::codecvt_utf8<wchar_t>;
			std::wstring_convert<convert_type, wchar_t> converter;
			std::string converted_fin = converter.to_bytes(fin);

			std::experimental::filesystem::v1::path abs = absolute(fin);
			this->dcms.push_back(abs.generic_wstring()); // all absolute paths for dcms(temps)

			DcmFileFormat dcmff(temp);
			dcmff.saveFile(converted_fin.c_str(), writeXfer, lengthEnc, grpLengthEnc, padEnc, OFstatic_cast(Uint32, filepad), OFstatic_cast(Uint32, itempad), writeMode);
		}

			delete temp;
	}

}

void Tiff2Dcm::mergeDCM(DcmDataset *resultObject)
{
	for (int i = 0; i < this->dcms.size(); i++)
	{
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		std::string converted_str = converter.to_bytes(this->dcms[i]);

	}

}

void Tiff2Dcm::cleanUP()
{
	for (auto file : this->paths) //close open files(tmps)
	{
		std::ifstream tmp(file);

		if (tmp.is_open())
		{
			tmp.close();
		}
	}

	for (auto file : this->dcms) //close open files(tmps)
	{
		std::ifstream tmp(file);

		if (tmp.is_open())
		{
			tmp.close();
		}
	}


	if (exists(this->pathData)) //delete all tmps
	{
		remove_all(this->pathData);
	}

	if (exists(this->pathDCM))
	{
		remove_all(this->pathDCM);
	}
}
