#include "Configuration.h"
#include "CommonFunctions.h"
#include "CommonRegexes.h"
#include "ConverterVersion.h"
#include "GameVersion.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <istream>
#include <stdexcept>
#include <string>

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
		Log(LogLevel::Info) << "EU5 savegame path: " << EU5SaveGamePath.string();
	});
	registerKeyword("EU5Directory", [this](std::istream& theStream) {
		EU5Path = commonItems::getString(theStream);
		Log(LogLevel::Info) << "EU5 path: " << EU5Path.string();
	});
	registerKeyword("EU5DocumentsDirectory", [this](std::istream& theStream) {
		EU5DocumentsPath = commonItems::getString(theStream);
		Log(LogLevel::Info) << "EU5 documents path: " << EU5DocumentsPath.string();
	});
	registerKeyword("Vic3Directory", [this](std::istream& theStream) {
		Vic3Path = commonItems::getString(theStream);
		Log(LogLevel::Info) << "Vic3 path: " << Vic3Path.string();
	});

	// ------- options

	registerKeyword("start_date", [this](std::istream& theStream) {
		const auto startDateString = commonItems::getString(theStream);
		configBlock.startDate = static_cast<STARTDATE>(std::stoi(startDateString));
		Log(LogLevel::Info) << "Start Date: " << startDateString;
	});
	registerKeyword("output_name", [this](std::istream& theStream) {
		outputName = commonItems::getString(theStream);
		Log(LogLevel::Info) << "Output Name: " << outputName.string();
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void Configuration::verifyEU5Path() const
{
	if (!commonItems::DoesFolderExist(EU5Path))
		throw std::runtime_error("EU5 path " + EU5Path.string() + " does not exist!");
	if (!commonItems::DoesFileExist(EU5Path / "binaries/eu5.exe") && !commonItems::DoesFileExist(EU5Path / "binaries/eu5"))
		throw std::runtime_error(EU5Path.string() + " does not contain Europa Universalis 5!");
	if (!commonItems::DoesFileExist(EU5Path / "game/in_game/map_data/definitions.txt"))
		throw std::runtime_error(EU5Path.string() + " does not appear to be a valid EU5 install!");
	Log(LogLevel::Info) << "\tEU5 install path is " << EU5Path.string();
}

void Configuration::verifyVic3Path()
{
	if (!commonItems::DoesFolderExist(Vic3Path))
		throw std::runtime_error("Vic3 path " + Vic3Path.string() + " does not exist!");
	if (!commonItems::DoesFileExist(Vic3Path / "binaries/victoria3.exe") && !commonItems::DoesFileExist(Vic3Path / "Vic3game") &&
		 !commonItems::DoesFileExist(Vic3Path / "binaries/victoria3"))
		throw std::runtime_error(Vic3Path.string() + " does not contain Victoria 3!");
	if (!commonItems::DoesFileExist(Vic3Path / "game/map_data/provinces.png"))
		throw std::runtime_error(Vic3Path.string() + " does not appear to be a valid Vic3 install!");
	Log(LogLevel::Info) << "\tVic3 install path is " << Vic3Path.string();
	Vic3Path = Vic3Path / "game"; // We're adding "/game/" since all we ever need from now on is in that subdirectory.
}

void Configuration::setOutputName()
{
	if (outputName.empty())
	{
		outputName = EU5SaveGamePath.stem();
	}

	outputName = outputName.filename();
	outputName = replaceCharacter(outputName.string(), '-');
	outputName = replaceCharacter(outputName.string(), ' ');
	outputName = commonItems::normalizeUTF8Path(outputName.string());

	Log(LogLevel::Info) << "Using output name " << outputName.string();
}

void Configuration::verifyEU5Version(const commonItems::ConverterVersion& converterVersion)
{
	EU5Version = GameVersion::extractVersionFromBranchTxt(EU5Path / "clausewitz_branch.txt");
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

void Configuration::verifyVic3Version(const commonItems::ConverterVersion& converterVersion)
{
	Vic3Version = GameVersion::extractVersionFromLauncher(Vic3Path / "../launcher/launcher-settings.json");
	if (!Vic3Version)
	{
		Log(LogLevel::Error) << "Vic3 version could not be determined, proceeding blind!";
		return;
	}

	Log(LogLevel::Info) << "Vic3 version: " << Vic3Version->toShortString();

	if (converterVersion.getMinTarget() > *Vic3Version)
	{
		Log(LogLevel::Error) << "Vic3 version is v" << Vic3Version->toShortString() << ", converter requires minimum v"
									<< converterVersion.getMinTarget().toShortString() << "!";
		throw std::runtime_error("Converter vs Vic3 installation mismatch!");
	}
	if (!converterVersion.getMaxTarget().isLargerishThan(*Vic3Version))
	{
		Log(LogLevel::Error) << "Vic3 version is v" << Vic3Version->toShortString() << ", converter requires maximum v"
									<< converterVersion.getMaxTarget().toShortString() << "!";
		throw std::runtime_error("Converter vs Vic3 installation mismatch!");
	}
}
