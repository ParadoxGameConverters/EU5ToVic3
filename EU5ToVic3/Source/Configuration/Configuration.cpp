#include "Configuration.h"
#include "CommonFunctions.h"
#include "CommonRegexes.h"
#include "GameVersion.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <fstream>

Configuration::Configuration(const commonItems::ConverterVersion& converterVersion)
{
	Log(LogLevel::Info) << "Reading configuration file";
	registerKeys();
	parseFile("configuration.txt");
	clearRegisteredKeywords();
	setOutputName();
	verifyEU5Path();
	verifyEU5Version(converterVersion);
	verifyVic3Path();
	verifyVic3Version(converterVersion);
	Log(LogLevel::Progress) << "3 %";
}

Configuration::Configuration(std::istream& theStream, const commonItems::ConverterVersion& converterVersion)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
	setOutputName();
	verifyEU5Path();
	verifyEU5Version(converterVersion);
	verifyVic3Path();
	verifyVic3Version(converterVersion);
}

void Configuration::registerKeys()
{
	// ------ config stuff

	registerKeyword("SaveGame", [this](std::istream& theStream) {
		EU5SaveGamePath = commonItems::getString(theStream);
		Log(LogLevel::Info) << "EU5 savegame path: " << EU5SaveGamePath;
	});
	registerKeyword("EU5directory", [this](std::istream& theStream) {
		EU5Path = commonItems::getString(theStream);
		Log(LogLevel::Info) << "EU5 path: " << EU5Path;
	});
	registerKeyword("EU5DocumentsDirectory", [this](std::istream& theStream) {
		EU5DocumentsPath = commonItems::getString(theStream);
		Log(LogLevel::Info) << "EU5 documents path: " << EU5DocumentsPath;
	});
	registerKeyword("Vic3directory", [this](std::istream& theStream) {
		Vic3Path = commonItems::getString(theStream);
		Log(LogLevel::Info) << "Vic3 path: " << Vic3Path;
	});

	// ------- options

	registerKeyword("start_date", [this](std::istream& theStream) {
		const auto startDateString = commonItems::getString(theStream);
		configBlock.startDate = static_cast<STARTDATE>(std::stoi(startDateString));
		Log(LogLevel::Info) << "Start Date: " << startDateString;
	});
	registerKeyword("output_name", [this](std::istream& theStream) {
		outputName = commonItems::getString(theStream);
		Log(LogLevel::Info) << "Output Name: " << outputName;
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void Configuration::verifyEU5Path() const
{
	if (!commonItems::DoesFolderExist(EU5Path))
		throw std::runtime_error("EU5 path " + EU5Path + " does not exist!");
	if (!commonItems::DoesFileExist(EU5Path + "/eu5.exe") && !commonItems::DoesFileExist(EU5Path + "/eu5") &&
		 !commonItems::DoesFolderExist(EU5Path + "/eu5.app"))
		throw std::runtime_error(EU5Path + " does not contain Europa Universalis 5!");
	if (!commonItems::DoesFileExist(EU5Path + "/map/positions.txt"))
		throw std::runtime_error(EU5Path + " does not appear to be a valid EU5 install!");
	Log(LogLevel::Info) << "\tEU5 install path is " << EU5Path;
}

void Configuration::verifyVic3Path()
{
	if (!commonItems::DoesFolderExist(Vic3Path))
		throw std::runtime_error("Vic3 path " + Vic3Path + " does not exist!");
	if (!commonItems::DoesFileExist(Vic3Path + "/binaries/victoria3.exe") && !commonItems::DoesFileExist(Vic3Path + "/Vic3game") &&
		 !commonItems::DoesFileExist(Vic3Path + "/binaries/victoria3"))
		throw std::runtime_error(Vic3Path + " does not contain Victoria 3!");
	if (!commonItems::DoesFileExist(Vic3Path + "/game/map_data/provinces.png"))
		throw std::runtime_error(Vic3Path + " does not appear to be a valid Vic3 install!");
	Log(LogLevel::Info) << "\tVic3 install path is " << Vic3Path;
	Vic3Path += "/game/"; // We're adding "/game/" since all we ever need from now on is in that subdirectory.
}

void Configuration::setOutputName()
{
	if (outputName.empty())
	{
		outputName = trimPath(EU5SaveGamePath);
	}

	outputName = trimExtension(outputName);
	outputName = replaceCharacter(outputName, '-');
	outputName = replaceCharacter(outputName, ' ');

	outputName = commonItems::normalizeUTF8Path(outputName);
	Log(LogLevel::Info) << "Using output name " << outputName;
}

void Configuration::verifyEU5Version(const commonItems::ConverterVersion& converterVersion) const
{
	const auto EU5Version = GameVersion::extractVersionFromLauncher(EU5Path + "/launcher-settings.json");
	if (!EU5Version)
	{
		Log(LogLevel::Error) << "EU5 version could not be determined, proceeding blind!";
		return;
	}

	Log(LogLevel::Info) << "EU5 version: " << EU5Version->toShortString();

	if (converterVersion.getMinSource() > *EU5Version)
	{
		Log(LogLevel::Error) << "EU5 version is v" << EU5Version->toShortString() << ", converter requires minimum v"
									<< converterVersion.getMinSource().toShortString() << "!";
		throw std::runtime_error("Converter vs EU5 installation mismatch!");
	}
	if (!converterVersion.getMaxSource().isLargerishThan(*EU5Version))
	{
		Log(LogLevel::Error) << "EU5 version is v" << EU5Version->toShortString() << ", converter requires maximum v"
									<< converterVersion.getMaxSource().toShortString() << "!";
		throw std::runtime_error("Converter vs EU5 installation mismatch!");
	}
}

void Configuration::verifyVic3Version(const commonItems::ConverterVersion& converterVersion) const
{
	const auto V3Version = GameVersion::extractVersionFromLauncher(Vic3Path + "../launcher/launcher-settings.json");
	if (!V3Version)
	{
		Log(LogLevel::Error) << "Vic3 version could not be determined, proceeding blind!";
		return;
	}

	Log(LogLevel::Info) << "Vic3 version: " << V3Version->toShortString();

	if (converterVersion.getMinTarget() > *V3Version)
	{
		Log(LogLevel::Error) << "Vic3 version is v" << V3Version->toShortString() << ", converter requires minimum v"
									<< converterVersion.getMinTarget().toShortString() << "!";
		throw std::runtime_error("Converter vs Vic3 installation mismatch!");
	}
	if (!converterVersion.getMaxTarget().isLargerishThan(*V3Version))
	{
		Log(LogLevel::Error) << "Vic3 version is v" << V3Version->toShortString() << ", converter requires maximum v"
									<< converterVersion.getMaxTarget().toShortString() << "!";
		throw std::runtime_error("Converter vs Vic3 installation mismatch!");
	}
}
