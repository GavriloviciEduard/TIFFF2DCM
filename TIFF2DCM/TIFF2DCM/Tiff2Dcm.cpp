#include "Tiff2Dcm.h"


Tiff2Dcm::Tiff2Dcm()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); //setup GDI+
	this->pathData = L"data"; //path for tmp
	this->cleanUP(); //cleanup just in case
}

Tiff2Dcm::~Tiff2Dcm()
{
	GdiplusShutdown(gdiplusToken); //close GDI+
	this->cleanUP(); //cleanup temp
}

void Tiff2Dcm::convertBMPtoDCM(const std::wstring pathIN, const std::wstring pathOUT)
{
	this->convertTIFFtoJPEG(pathIN);

	
	OFString outputFile = this->convertWstring(pathOUT).c_str();

	if (this->paths.size()) // if converted with succes
	{
		DJEncoderRegistration::registerCodecs();

		Image2Dcm i2d;
		I2DOutputPlug *outPlug = NULL;
		I2DImgSource *inputPlug = NULL;
		E_GrpLenEncoding grpLengthEnc = EGL_recalcGL;
		E_EncodingType lengthEnc = EET_ExplicitLength;
		E_PaddingEncoding padEnc = EPD_noChange;
		OFCmdUnsignedInt filepad = 0;
		OFCmdUnsignedInt itempad = 0;
		E_FileWriteMode writeMode = EWM_fileformat;
		OFList<OFString> overrideKeys;
		E_TransferSyntax writeXfer;
		DcmFileFormat file;


		inputPlug = new I2DJpegSource();
		outPlug = new I2DOutputPlugNewSC();
		writeMode = EWM_fileformat;
		grpLengthEnc = EGL_recalcGL;
		lengthEnc = EET_ExplicitLength;
		i2d.setOverrideKeys(overrideKeys);
		OFBool insertLatin1 = OFTrue;
		OFBool insertType2 = OFTrue;
		OFBool doChecks = OFTrue;
		i2d.setValidityChecking(doChecks, insertType2, true);
		outPlug->setValidityChecking(doChecks, insertType2, true);
		inputPlug->setImageFile(this->convertWstring(this->paths[0]).c_str());

		if (!dcmDataDict.isDictionaryLoaded())
		{
			return;
		}

		DcmDataset *resultObject = NULL;
		OFCondition cond = i2d.convert(inputPlug, outPlug, resultObject, writeXfer);//create base file first
		this->CreateHeaderImage(resultObject);
		long frames = 0;


		//TODO CREATE HEADER FROM TXT BEFORE CONSTRUCTING REST OF FILE
		
		for (int i = 1; i < this->paths.size(); i++)
		{

			OFCondition result;
			DcmElement* element = NULL;
			result = resultObject->findAndGetElement(DCM_PixelData, element);
			DcmPixelData *dpix = OFstatic_cast(DcmPixelData*, element);
			DcmPixelSequence *dseq = NULL;
			E_TransferSyntax xferSyntax = EXS_Unknown;
			const DcmRepresentationParameter *rep = NULL;
			dpix->getOriginalRepresentationKey(xferSyntax, rep);
			result = dpix->getEncapsulatedRepresentation(xferSyntax, rep, dseq); // BMP error not pixel representation ?
			
			if (result.good())
			{
				DcmPixelItem *newItem = new DcmPixelItem(DcmTag(DCM_Item, EVR_OB));
				this->extractPixelData(this->paths[i]);
				newItem->putUint8Array((Uint8 *)this->pixeldata.data(), this->pixeldata.size());
				result = dseq->append((DcmItem*)newItem);
				resultObject->findAndGetLongInt(DCM_NumberOfFrames, frames);
				frames++;
				std::string sFrames = std::to_string(frames);
				resultObject->putAndInsertString(DCM_NumberOfFrames, sFrames.c_str());
				this->nrpixels = 0;
			}
		}
		
	
		if (cond.good())// Save
		{

			DcmFileFormat dcmff(resultObject);
			dcmff.saveFile(outputFile.c_str(), writeXfer, lengthEnc, grpLengthEnc, padEnc, OFstatic_cast(Uint32, filepad), OFstatic_cast(Uint32, itempad), writeMode);
		}

		delete outPlug;
		outPlug = NULL;
		delete inputPlug;
		inputPlug = NULL;
		delete resultObject;
		resultObject = NULL;
		DJEncoderRegistration::cleanup();
	}

}

void Tiff2Dcm::convertTIFFtoJPEG(const std::wstring path)
{
	
	std::ifstream file(path.c_str()); //file
	std::string str(path.begin(),path.end()); //string to check extension

	if (!exists(this->pathData)) // create directory for tmps if it does not exist
		create_directory(this->pathData);

	if (file.good() && (str.substr(str.find_last_of(".") + 1) == "tiff" || str.substr(str.find_last_of(".") + 1) == "tif") && exists(this->pathData))
	{
		Image* temp = Image::FromFile(path.c_str(), 1); //read tiff in image
		int count = temp->GetFrameCount(&FrameDimensionPage); // get frames of tiff

		for (int i = 0; i < count; i++) // for all frames in tiff
		{

			temp->SelectActiveFrame(&FrameDimensionPage, i); // current frame
			
			std::wstring name(L"tmp");
			std::wstring type(L".jpg");
			std::wstring nr = std::to_wstring(i);
			std::wstring fin = this->pathData + L"//" + name + nr + type;  // path to save jpeg

			CLSID bmpClsid;
			CLSIDFromString(L"{557cf401-1a04-11d3-9a73-0000f81ef32e}", &bmpClsid);
			temp->Save(fin.c_str(), &bmpClsid, NULL); //save current frame as jpeg 

			std::experimental::filesystem::v1::path abs = absolute(fin);
			this->paths.push_back(abs.generic_wstring()); // all absolute paths for frames(tmps)
		}


		//cleanup
		delete temp;
		file.close();
	}	
}

void Tiff2Dcm::extractPixelData(const std::wstring path)
{	
	std::ifstream input(path.c_str(), std::ios::binary);
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
	this->pixeldata = buffer;
}

OFCondition Tiff2Dcm::CreateHeaderImage(DcmDataset *dcmDataSet)
{
	OFCondition condResult;
	//condResult = dcmDataSet->putAndInsertString(DCM_PatientName, strPatientName.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_PatientID, strPatientID.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_PatientBirthDate, strPatientDOB.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_PatientSex, strPatientSex.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_PatientSize, strPatientHeight.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_StudyInstanceUID, strStudyUID.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_StudyDate, strStudyDate.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_StudyTime, strStudyTime.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_StudyDescription, strStudyDescription.c_str());
	if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_NumberOfFrames, "1");
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_InstitutionAddress, strInstitutionAdress.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_InstitutionName, strInstitutionName.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_PatientAge, strPatientAge.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_Manufacturer, strManufacturer.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_ManufacturerModelName, strModel.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_SeriesNumber, strSeriesNumber.c_str());

	//if (strSeriesUID == "")
	//{
		//char uid[100];
		//strSeriesUID = dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
	//}

	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_SeriesInstanceUID, strSeriesUID.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_SeriesDate, strSeriesDate.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_SeriesTime, strSeriesTime.c_str());
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_SeriesDescription, strSeriesDescription.c_str());

	//if (strSOPInstanceUID == "")
	//{
		//char uid[100];
		//strSOPInstanceUID = dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
	//}


	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_SOPInstanceUID, strSOPInstanceUID.c_str());
	if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_SOPClassUID, UID_UltrasoundImageStorage);
	//if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_Modality, strModality.c_str());
	return condResult;
}

std::string Tiff2Dcm::convertWstring(std::wstring wstr)
{
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	std::string converted_str = converter.to_bytes(wstr);

	return converted_str;
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

	if (exists(this->pathData)) //delete all tmps
	{
		remove_all(this->pathData);
	}

}

