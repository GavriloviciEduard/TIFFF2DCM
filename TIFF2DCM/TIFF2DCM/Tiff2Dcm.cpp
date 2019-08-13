#include "Tiff2Dcm.h"


Tiff2Dcm::Tiff2Dcm()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	this->pathData = L"data";
	this->BMPData = L"dataBMP";

	if (exists(this->pathData))
		remove_all(this->pathData);

	if (exists(this->BMPData))
		remove_all(this->pathData);
}

Tiff2Dcm::~Tiff2Dcm()
{
	GdiplusShutdown(gdiplusToken);
}

void Tiff2Dcm::convertBMPtoDCM(const std::wstring path, const OFString& outputFile)
{
	this->convertTIFFtoBMP(path);

	this->saveTempBMP();

	if (this->paths.size())
	{

		//// Main class for controlling conversion
		Image2Dcm i2d;
		//// Output plugin to use (i.e. SOP class to write)
		I2DOutputPlug *outPlug = NULL;
		//// Input plugin to use (i.e. file format to read)
		I2DImgSource *inputPlug = NULL;
		//// Group length encoding mode for output DICOM file
		E_GrpLenEncoding grpLengthEnc = EGL_recalcGL;
		//// Item and Sequence encoding mode for output DICOM file
		E_EncodingType lengthEnc = EET_ExplicitLength;
		//// Padding mode for output DICOM file
		E_PaddingEncoding padEnc = EPD_noChange;
		//// File pad length for output DICOM file
		OFCmdUnsignedInt filepad = 0;
		//// Item pad length for output DICOM file
		OFCmdUnsignedInt itempad = 0;
		//// Write only pure dataset, i.e. without meta header
		E_FileWriteMode writeMode = EWM_fileformat;
		//// Override keys are applied at the very end of the conversion "pipeline"
		OFList<OFString> overrideKeys;
		//// The transfer syntax proposed to be written by output plugin
		E_TransferSyntax writeXfer;
		DcmFileFormat file;


		inputPlug = new I2DBmpSource();
		outPlug = new I2DOutputPlugSC();
		writeMode = EWM_fileformat;
		grpLengthEnc = EGL_recalcGL;
		lengthEnc = EET_ExplicitLength;

		i2d.setOverrideKeys(overrideKeys);

		//// Test for ISO Latin 1 option
		OFBool insertLatin1 = OFTrue;

		//// evaluate validity checking options
		OFBool insertType2 = OFTrue;
		OFBool doChecks = OFTrue;



		i2d.setValidityChecking(doChecks, insertType2, true);
		outPlug->setValidityChecking(doChecks, insertType2, true);


		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		std::string converted_str = converter.to_bytes(this->paths[0]);

		inputPlug->setImageFile(converted_str.c_str());

		///* make sure data dictionary is loaded */
		if (!dcmDataDict.isDictionaryLoaded())
		{
			return;
		}

		DcmDataset *resultObject = NULL;
		OFCondition cond = i2d.convert(inputPlug, outPlug, resultObject, writeXfer);

		for (int i = 1; i < this->paths.size(); i++)
		{
			using convert_type = std::codecvt_utf8<wchar_t>;
			std::wstring_convert<convert_type, wchar_t> converter;
			std::string converted_str = converter.to_bytes(this->paths[i]);

			//i2d.readAndInsertPixelData(converted_str,)
		}

		



		// Save
		if (cond.good())
		{

			DcmFileFormat dcmff(resultObject);
			cond = dcmff.saveFile(outputFile.c_str(), writeXfer, lengthEnc, grpLengthEnc, padEnc, OFstatic_cast(Uint32, filepad), OFstatic_cast(Uint32, itempad), writeMode);
		}

		//	//// Cleanup and return
		delete outPlug; outPlug = NULL;
		delete inputPlug; inputPlug = NULL;
		delete resultObject; resultObject = NULL;

		//	//return cond;
		
	}

}

void Tiff2Dcm::convertTIFFtoBMP(const std::wstring path)
{
	
	std::ifstream file(path.c_str());
	std::string str(path.begin(),path.end());

	if (!exists(this->pathData))
		create_directory(this->pathData);

	if (file.good() && (str.substr(str.find_last_of(".") + 1) == "tiff" || str.substr(str.find_last_of(".") + 1) == "tif") && exists(this->pathData))
	{
		Bitmap* temp = Bitmap::FromFile(path.c_str(), 1);
		int count = temp->GetFrameCount(&FrameDimensionPage);

		for (int i = 0; i < count; i++)
		{
		    temp->SelectActiveFrame(&FrameDimensionPage, i);
			CLSID bmpClsid;
			CLSIDFromString(L"{557cf400-1a04-11d3-9a73-0000f81ef32e}", &bmpClsid);
			std::wstring name(L"tmp");
			std::wstring nr = std::to_wstring(i);
			std::wstring fin = this->pathData + L"//" + name + nr;
			temp->Save(fin.c_str(), &bmpClsid, NULL);
			this->bitmaps.push_back(Bitmap::FromFile(fin.c_str()));
		}

		//save to file test
		//CLSID pngClsid;
		//CLSIDFromString(L"{557cf400-1a04-11d3-9a73-0000f81ef32e}", &pngClsid);
		//this->bitmaps[8]->Save(L"test.bmp", &pngClsid, NULL);

		delete temp;
	}
	
}

void Tiff2Dcm::saveTempBMP()
{
	if (!exists(this->BMPData))
		create_directory(this->BMPData);

	if (exists(this->pathData))
	{
		for (int i = 0; i < this->bitmaps.size(); i++)
		{
			std::wstring type(L".bmp");
			std::wstring name(L"tmp");
			std::wstring nr = std::to_wstring(i);
			std::wstring fin = this->BMPData + +L"//"+ name + nr + type;
			CLSID pngClsid;
			CLSIDFromString(L"{557cf400-1a04-11d3-9a73-0000f81ef32e}", &pngClsid);
			this->bitmaps[i]->Save(fin.c_str(), &pngClsid, NULL);
			path abs = absolute(fin);
			this->paths.push_back(abs.generic_wstring());
		}
	}

	this->bitmaps.clear();
}
