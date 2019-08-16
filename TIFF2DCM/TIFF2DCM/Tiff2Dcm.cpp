#include "Tiff2Dcm.h"

Tiff2Dcm::Tiff2Dcm()
{
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); //setup GDI+
	this->pathData = L"data"; //path for tmp
	this->cleanUP(); //cleanup just in case
}

//========================================================================================================================
Tiff2Dcm::~Tiff2Dcm()
{
	Gdiplus::GdiplusShutdown(gdiplusToken); //close GDI+
	this->cleanUP(); //cleanup temp
}

//========================================================================================================================
std::string convertWstring(std::wstring wstr)
{
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	std::string converted_str = converter.to_bytes(wstr);

	return converted_str;
}

//========================================================================================================================
std::vector<std::string> split(std::string str, const std::string& delim)
{
	std::vector<std::string> str_s;
	char*s = (char*)str.c_str();
	char* p;

	p = strtok(s, delim.c_str());

	while (p)
	{
		str_s.push_back(p);
		p = strtok(0, delim.c_str());
	}

	return str_s;
}

//========================================================================================================================
size_t GetSizeOfFile(const std::wstring& path)
{
	struct _stat fileinfo;
	_wstat(path.c_str(), &fileinfo);
	return fileinfo.st_size;
}

//========================================================================================================================
std::wstring readFile(const std::wstring& path)
{
	std::wstring buffer;
	FILE* file = _wfopen(path.c_str(), L"rtS, ccs=UTF-8");

	if (file)
	{
		size_t filesize = GetSizeOfFile(path.c_str());

		if (filesize > 0)
		{
			buffer.resize(filesize);
			size_t wchars_read = fread(&(buffer.front()), sizeof(wchar_t), filesize, file);
			buffer.resize(wchars_read);
			buffer.shrink_to_fit();
		}

		fclose(file);
		return buffer;
	}

	else
	{
		return std::wstring(L"error");
	}
}

//========================================================================================================================
void Tiff2Dcm::convertJPEGtoDCM(const std::wstring& pathIN, const std::wstring& pathDATA, const std::wstring& pathOUT)
{
	this->convertTIFFtoJPEG(pathIN);

	if (this->paths.size()) // if converted with succes
	{
		DJEncoderRegistration::registerCodecs(); //register codecs for jpeg encoding

		//init
		Image2Dcm i2d;
		I2DOutputPlug *outPlug = new I2DOutputPlugNewSC();
		I2DImgSource *inputPlug = new I2DJpegSource();
		DcmDataset *resultObject = NULL;
		E_GrpLenEncoding grpLengthEnc = EGL_recalcGL;
		E_EncodingType lengthEnc = EET_ExplicitLength;
		E_PaddingEncoding padEnc = EPD_noChange;
		OFCmdUnsignedInt filepad = 0;
		OFCmdUnsignedInt itempad = 0;
		E_FileWriteMode writeMode = EWM_fileformat;
		OFList<OFString> overrideKeys;
		E_TransferSyntax writeXfer;
		DcmFileFormat file;
		OFString outputFile = convertWstring(pathOUT).c_str();
		long frames = 0;
		OFBool insertType2 = OFTrue;
		OFBool doChecks = OFTrue;

		i2d.setOverrideKeys(overrideKeys);
		i2d.setValidityChecking(doChecks, insertType2, true);
		outPlug->setValidityChecking(doChecks, insertType2, true);
		inputPlug->setImageFile(convertWstring(this->paths[0]).c_str());

		if (!dcmDataDict.isDictionaryLoaded()) //if dictionary not loaded corectlly
			return;
	
		OFCondition cond = i2d.convert(inputPlug, outPlug, resultObject, writeXfer); //create base file first
		OFCondition okread = this->readTags(pathDATA);

		if (okread.bad())
			exit(3);

		OFCondition okInserted = this->insertTags(resultObject);
		this->insertFrames(resultObject, frames);

		if (cond.good() && okInserted.good()) //Save dcm file
		{
			DcmFileFormat dcmff(resultObject);
			dcmff.saveFile(outputFile.c_str(), writeXfer, lengthEnc, grpLengthEnc, padEnc, OFstatic_cast(Uint32, filepad), OFstatic_cast(Uint32, itempad), writeMode);
		}

		else
			exit(2); //could not create dcm
		
		//cleanup
		delete outPlug;
		outPlug = NULL;
		delete inputPlug;
		inputPlug = NULL;
		delete resultObject;
		resultObject = NULL;
		DJEncoderRegistration::cleanup();
	}
}

//========================================================================================================================
void Tiff2Dcm::convertTIFFtoJPEG(const std::wstring& path)
{
	
	std::ifstream file(path.c_str()); //file
	std::string str(path.begin(),path.end()); //string to check extension

	if (!std::experimental::filesystem::v1::exists(this->pathData)) // create directory for tmps if it does not exist
		std::experimental::filesystem::v1::create_directory(this->pathData);

	if (file.good() && (str.substr(str.find_last_of(".") + 1) == "tiff" || str.substr(str.find_last_of(".") + 1) == "tif") && std::experimental::filesystem::v1::exists(this->pathData))
	{
		Gdiplus::Image* temp = Gdiplus::Image::FromFile(path.c_str(), 1); //read tiff in image
		int count = temp->GetFrameCount(&Gdiplus::FrameDimensionPage); // get frames of tiff

		for (int i = 0; i < count; i++) // for all frames in tiff
		{
			temp->SelectActiveFrame(&Gdiplus::FrameDimensionPage, (uint)i); // current frame

			std::wstring name(L"tmp");
			std::wstring type(L".jpg");
			std::wstring nr = std::to_wstring(i);
			std::wstring fin = this->pathData + L"//" + name + nr + type;  // path to save jpeg

			CLSID bmpClsid;
			CLSIDFromString(L"{557cf401-1a04-11d3-9a73-0000f81ef32e}", &bmpClsid);
			temp->Save(fin.c_str(), &bmpClsid, NULL); //save current frame as jpeg 

			std::experimental::filesystem::v1::path abs = std::experimental::filesystem::v1::absolute(fin);
			this->paths.push_back(abs.generic_wstring()); // all absolute paths for frames(tmps)
		}

		//cleanup
		delete temp;
		file.close();
	}

	else
		exit(1); //file not found
}

//========================================================================================================================
void Tiff2Dcm::extractPixelData(const std::wstring& path)
{	
	std::ifstream input(path.c_str(), std::ios::binary);
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
	this->pixelData = buffer;
}

//========================================================================================================================
std::string Tiff2Dcm::findTag(const std::string& search)
{
	std::string res;
	std::unordered_map<std::string, std::string>::iterator it;

	if (search == "name")
	{
		it = this->tags.find("name");
		if (it != this->tags.end())
			res = it->second + " ";

		it = this->tags.find("surname");
		if (it != this->tags.end())
			res += it->second;

		return res;
	}

	else if (search == "patId")
	{
		it = this->tags.find("PatId");
		if (it != this->tags.end())
			res = it->second;

		return res;
	}

	else if (search == "dateBirth")
	{
		it = this->tags.find("DateBirth");
		if (it != this->tags.end())
			res = it->second;

		return res;
	}

	else if (search == "sex")
	{
		it = this->tags.find("Sex");
		if (it != this->tags.end())
			res = it->second;

		return res;
	}

	else if (search == "description")
	{
		it = this->tags.find("Comments");
		if (it != this->tags.end())
			res = it->second;

		return res;
	}

	else if (search == "patAddress")
	{
		it = this->tags.find("Address1");
		if (it != this->tags.end())
			res = it->second;

		return res;
	}

	else if (search == "perAddress")
	{
		it = this->tags.find("Address2");
		if (it != this->tags.end())
			res = it->second;

		return res;
	}

	else if (search == "operator")
	{
		it = this->tags.find("OperatorName");
		if (it != this->tags.end())
			res = it->second;

		return res;
	}

	else if (search == "insurance")
	{
		it = this->tags.find("Insurance");
		if (it != this->tags.end())
			res = it->second;

		return res;
	}

	else if (search == "phone")
	{
		it = this->tags.find("PhoneHome");
		if (it != this->tags.end())
			res = it->second + " ";

		it = this->tags.find("PhoneWork");
		if (it != this->tags.end())
			res += it->second + " ";

		it = this->tags.find("PhoneCell");
		if (it != this->tags.end())
			res += it->second;

		return res;
	}

	return res; //TO DO

}

//========================================================================================================================
OFCondition Tiff2Dcm::readTags(const std::wstring& path)
{
	std::wstring tagsFile = readFile(path.c_str());
	std::wstringstream stream(tagsFile);

	if (tagsFile != L"error")
	{
		std::wstring line;

		while (std::getline(stream, line))
		{
			std::vector<std::string> temp = split(convertWstring(line), "=");

			if (temp.size() == 2)
				this->tags.insert(std::pair<std::string, std::string>(temp[0], temp[1]));

			else
				this->tags.insert(std::pair<std::string, std::string>(temp[0], ""));

			temp.clear();
		}

		return OFCondition(EC_Normal);
	}

	else
		return OFCondition(EC_InvalidFilename);
}

//========================================================================================================================
void Tiff2Dcm::insertFrames(DcmDataset *dcmDataSet, long& frames)
{
	for (int i = 1; i < this->paths.size(); i++)
	{
		OFCondition result;
		DcmElement* element = NULL;
		result = dcmDataSet->findAndGetElement(DCM_PixelData, element);
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
			newItem->putUint8Array(this->pixelData.data(), (uint)this->pixelData.size());
			result = dseq->append((DcmItem*)newItem);
			dcmDataSet->findAndGetLongInt(DCM_NumberOfFrames, frames);
			frames++;
			std::string sFrames = std::to_string(frames);
			dcmDataSet->putAndInsertString(DCM_NumberOfFrames, sFrames.c_str());
			this->nrPixels = 0;
		}

		else
			exit(4); // failed to insert frames abort
	}
}

//========================================================================================================================
OFCondition Tiff2Dcm::insertTags(DcmDataset *dcmDataSet)
{
	OFCondition condResult;

	condResult = dcmDataSet->putAndInsertString(DCM_PatientName,findTag("name").c_str());

	if (condResult.good())
		condResult = dcmDataSet->putAndInsertString(DCM_PatientID, findTag("patId").c_str());

	if (condResult.good()) 
		condResult = dcmDataSet->putAndInsertString(DCM_PatientBirthDate, findTag("dateBirth").c_str());

	if (condResult.good())
		condResult = dcmDataSet->putAndInsertString(DCM_PatientSex, findTag("sex").c_str());

	if (condResult.good()) condResult = dcmDataSet->putAndInsertString(DCM_StudyDescription, findTag("description").c_str()); //?

	if (condResult.good())
		condResult = dcmDataSet->putAndInsertString(DCM_NumberOfFrames, "1");

	if (condResult.good())
		condResult = dcmDataSet->putAndInsertString(DCM_PatientAddress, findTag("patAddress").c_str()); // ?

	if (condResult.good())
		condResult = dcmDataSet->putAndInsertString(DCM_PersonAddress, findTag("perAddress").c_str()); // ?

	if (condResult.good())
		condResult = dcmDataSet->putAndInsertString(DCM_OperatorsName, findTag("operator").c_str());

	if (condResult.good())
		condResult = dcmDataSet->putAndInsertString(DCM_SOPClassUID, UID_UltrasoundImageStorage);

	if (condResult.good())
		condResult = dcmDataSet->putAndInsertString(DCM_RETIRED_InsurancePlanIdentification, findTag("insurance").c_str()); // ?

	if (condResult.good())
		condResult = dcmDataSet->putAndInsertString(DCM_OperatorsName, findTag("phone").c_str());

	//QuickId = aici este pus CNP - ul (?)
	//FiscalCode = (?)
	//szName=aaaaaaaaa (?)
	//szSurname=bbbbbbbbbb (?)
	//MidIni= (?)
	//BornPlace= (?)
	//Email= (?)
	//City= (?)
	//State = (? )
	//Zip = (?)
	//DateLastImage= (?)

	return condResult;
}

//========================================================================================================================
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

	if (std::experimental::filesystem::v1::exists(this->pathData)) //delete all tmps
	{
		std::experimental::filesystem::v1::remove_all(this->pathData);
	}
}

