#include "Tiff2Dcm.h"


Tiff2Dcm::Tiff2Dcm()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

Tiff2Dcm::~Tiff2Dcm()
{
	delete this->bitmap;
	GdiplusShutdown(gdiplusToken);
}

void Tiff2Dcm::convertBMPtoDCM(const std::wstring path)
{
	this->convertTIFFtoBMP(path);

	//
	//// Parse command line and exclusive options
	//prepareCmdLineArgs(argc, argv, OFFIS_CONSOLE_APPLICATION);
	//OFConsoleApplication app(OFFIS_CONSOLE_APPLICATION, "Convert standard image formats into DICOM format", rcsid);
	//if (app.parseCommandLine(cmd, argc, argv))
	//{
	//	/* check exclusive options first */
	//	if (cmd.hasExclusiveOption())
	//	{
	//		if (cmd.findOption("--version"))
	//		{
	//			app.printHeader(OFTrue /*print host identifier*/);
	//			exit(0);
	//		}
	//	}
	//}

	///* print resource identifier */
	//OFLOG_DEBUG(img2dcmLogger, rcsid << OFendl);

	//// Main class for controlling conversion
	//Image2Dcm i2d;
	//// Output plugin to use (i.e. SOP class to write)
	//I2DOutputPlug *outPlug = NULL;
	//// Input plugin to use (i.e. file format to read)
	//I2DImgSource *inputPlug = NULL;
	//// Group length encoding mode for output DICOM file
	//E_GrpLenEncoding grpLengthEnc = EGL_recalcGL;
	//// Item and Sequence encoding mode for output DICOM file
	//E_EncodingType lengthEnc = EET_ExplicitLength;
	//// Padding mode for output DICOM file
	//E_PaddingEncoding padEnc = EPD_noChange;
	//// File pad length for output DICOM file
	//OFCmdUnsignedInt filepad = 0;
	//// Item pad length for output DICOM file
	//OFCmdUnsignedInt itempad = 0;
	//// Write only pure dataset, i.e. without meta header
	//E_FileWriteMode writeMode = EWM_fileformat;
	//// Override keys are applied at the very end of the conversion "pipeline"
	//OFList<OFString> overrideKeys;
	//// The transfer syntax proposed to be written by output plugin
	//E_TransferSyntax writeXfer;

	//// Parse rest of command line options
	//OFLog::configureFromCommandLine(cmd, app);

	//OFString pixDataFile, outputFile, tempStr;
	//cmd.getParam(1, tempStr);

	//if (tempStr.empty())
	//{
	//	OFLOG_ERROR(img2dcmLogger, "No image input filename specified");
	//	return EC_IllegalCall;
	//}
	//else
	//	pixDataFile = tempStr;

	//cmd.getParam(2, tempStr);
	//if (tempStr.empty())
	//{
	//	OFLOG_ERROR(img2dcmLogger, "No DICOM output filename specified");
	//	return EC_IllegalCall;
	//}
	//else
	//	outputFile = tempStr;

	//if (cmd.findOption("--input-format"))
	//{
	//	app.checkValue(cmd.getValue(tempStr));
	//	if (tempStr == "JPEG")
	//	{
	//		inputPlug = new I2DJpegSource();
	//	}
	//	else if (tempStr == "BMP")
	//	{
	//		inputPlug = new I2DBmpSource();
	//	}
	//	else
	//	{
	//		return makeOFCondition(OFM_dcmdata, 18, OF_error, "No plugin for selected input format available");
	//	}
	//	if (!inputPlug)
	//	{
	//		return EC_MemoryExhausted;
	//	}
	//}
	//else // default is JPEG
	//{
	//	inputPlug = new I2DJpegSource();
	//}
	//OFLOG_INFO(img2dcmLogger, OFFIS_CONSOLE_APPLICATION ": Instantiated input plugin: " << inputPlug->inputFormat());

	//// Find out which plugin to use
	//cmd.beginOptionBlock();
	//if (cmd.findOption("--sec-capture"))
	//	outPlug = new I2DOutputPlugSC();
	//if (cmd.findOption("--vl-photo"))
	//{
	//	outPlug = new I2DOutputPlugVLP();
	//}
	//if (cmd.findOption("--new-sc"))
	//	outPlug = new I2DOutputPlugNewSC();
	//cmd.endOptionBlock();
	//if (!outPlug) // default is the old Secondary Capture object
	//	outPlug = new I2DOutputPlugSC();
	//if (outPlug == NULL) return EC_MemoryExhausted;
	//OFLOG_INFO(img2dcmLogger, OFFIS_CONSOLE_APPLICATION ": Instantiated output plugin: " << outPlug->ident());

	//cmd.beginOptionBlock();
	//if (cmd.findOption("--write-file"))    writeMode = EWM_fileformat;
	//if (cmd.findOption("--write-dataset")) writeMode = EWM_dataset;
	//cmd.endOptionBlock();

	//cmd.beginOptionBlock();
	//if (cmd.findOption("--group-length-recalc")) grpLengthEnc = EGL_recalcGL;
	//if (cmd.findOption("--group-length-create")) grpLengthEnc = EGL_withGL;
	//if (cmd.findOption("--group-length-remove")) grpLengthEnc = EGL_withoutGL;
	//cmd.endOptionBlock();

	//cmd.beginOptionBlock();
	//if (cmd.findOption("--length-explicit"))  lengthEnc = EET_ExplicitLength;
	//if (cmd.findOption("--length-undefined")) lengthEnc = EET_UndefinedLength;
	//cmd.endOptionBlock();

	//cmd.beginOptionBlock();
	//if (cmd.findOption("--padding-off"))
	//{
	//	filepad = 0;
	//	itempad = 0;
	//}
	//else if (cmd.findOption("--padding-create"))
	//{
	//	OFCmdUnsignedInt opt_filepad; OFCmdUnsignedInt opt_itempad;
	//	app.checkValue(cmd.getValueAndCheckMin(opt_filepad, 0));
	//	app.checkValue(cmd.getValueAndCheckMin(opt_itempad, 0));
	//	itempad = opt_itempad;
	//	filepad = opt_filepad;
	//}
	//cmd.endOptionBlock();

	//// create override attribute dataset (copied from findscu code)
	//if (cmd.findOption("--key", 0, OFCommandLine::FOM_FirstFromLeft))
	//{
	//	const char *ovKey = NULL;
	//	do {
	//		app.checkValue(cmd.getValue(ovKey));
	//		overrideKeys.push_back(ovKey);
	//	} while (cmd.findOption("--key", 0, OFCommandLine::FOM_NextFromLeft));
	//}
	//i2d.setOverrideKeys(overrideKeys);

	//// Test for ISO Latin 1 option
	//OFBool insertLatin1 = OFTrue;
	//cmd.beginOptionBlock();
	//if (cmd.findOption("--latin1"))
	//	insertLatin1 = OFTrue;
	//if (cmd.findOption("--no-latin1"))
	//	insertLatin1 = OFFalse;
	//cmd.endOptionBlock();
	//i2d.setISOLatin1(insertLatin1);

	//// evaluate validity checking options
	//OFBool insertType2 = OFTrue;
	//OFBool inventType1 = OFTrue;
	//OFBool doChecks = OFTrue;
	//cmd.beginOptionBlock();
	//if (cmd.findOption("--no-checks"))
	//	doChecks = OFFalse;
	//if (cmd.findOption("--do-checks"))
	//	doChecks = OFTrue;
	//cmd.endOptionBlock();

	//cmd.beginOptionBlock();
	//if (cmd.findOption("--insert-type2"))
	//	insertType2 = OFTrue;
	//if (cmd.findOption("--no-type2-insert"))
	//	insertType2 = OFFalse;
	//cmd.endOptionBlock();

	//cmd.beginOptionBlock();
	//if (cmd.findOption("--invent-type1"))
	//	inventType1 = OFTrue;
	//if (cmd.findOption("--no-type1-invent"))
	//	inventType1 = OFFalse;
	//cmd.endOptionBlock();
	//i2d.setValidityChecking(doChecks, insertType2, inventType1);
	//outPlug->setValidityChecking(doChecks, insertType2, inventType1);

	//// evaluate --xxx-from options and transfer syntax options
	//OFCondition cond;
	//cond = evaluateFromFileOptions(cmd, i2d);
	//if (cond.bad())
	//{
	//	delete outPlug; outPlug = NULL;
	//	delete inputPlug; inputPlug = NULL;
	//	return cond;
	//}

	//if (inputPlug->inputFormat() == "JPEG")
	//{
	//	I2DJpegSource *jpgSource = OFstatic_cast(I2DJpegSource*, inputPlug);
	//	if (!jpgSource)
	//	{
	//		delete outPlug; outPlug = NULL;
	//		delete inputPlug; inputPlug = NULL;
	//		return EC_MemoryExhausted;
	//	}
	//	if (cmd.findOption("--disable-progr"))
	//		jpgSource->setProgrSupport(OFFalse);
	//	if (cmd.findOption("--disable-ext"))
	//		jpgSource->setExtSeqSupport(OFFalse);
	//	if (cmd.findOption("--insist-on-jfif"))
	//		jpgSource->setInsistOnJFIF(OFTrue);
	//	if (cmd.findOption("--keep-appn"))
	//		jpgSource->setKeepAPPn(OFTrue);
	//}
	//inputPlug->setImageFile(pixDataFile);

	///* make sure data dictionary is loaded */
	//if (!dcmDataDict.isDictionaryLoaded())
	//{
	//	OFLOG_WARN(img2dcmLogger, "no data dictionary loaded, check environment variable: "
	//		<< DCM_DICT_ENVIRONMENT_VARIABLE);
	//}

	//DcmDataset *resultObject = NULL;
	//OFLOG_INFO(img2dcmLogger, OFFIS_CONSOLE_APPLICATION ": Starting image conversion");
	//cond = i2d.convert(inputPlug, outPlug, resultObject, writeXfer);

	//// Save
	//if (cond.good())
	//{
	//	OFLOG_INFO(img2dcmLogger, OFFIS_CONSOLE_APPLICATION ": Saving output DICOM to file " << outputFile);
	//	DcmFileFormat dcmff(resultObject);
	//	cond = dcmff.saveFile(outputFile.c_str(), writeXfer, lengthEnc, grpLengthEnc, padEnc, OFstatic_cast(Uint32, filepad), OFstatic_cast(Uint32, itempad), writeMode);
	//}

	//// Cleanup and return
	//delete outPlug; outPlug = NULL;
	//delete inputPlug; inputPlug = NULL;
	//delete resultObject; resultObject = NULL;

	//return cond;


	

}

void Tiff2Dcm::convertTIFFtoBMP(const std::wstring path)
{
	std::ifstream file(path.c_str());

	if (file.good())
		this->bitmap = Bitmap::FromFile(path.c_str(), 1);

	//save to file
	CLSID pngClsid;
	CLSIDFromString(L"{557cf400-1a04-11d3-9a73-0000f81ef32e}", &pngClsid);
	this->bitmap->Save(L"test.bmp", &pngClsid, NULL);	
}

