#include "World.h"
#include "CommonRegexes.h"
#include "Configuration.h"
#include "ConverterVersion.h"
#include "GameVersion.h"
#include "Log.h"
#include "ParserHelpers.h"
#include "rakaly.h"
#include "zip.h"
#include <filesystem>
#include <fstream>
#include <ios>
#include <istream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

EU5::World::World(const std::shared_ptr<Configuration>& theConfiguration, const commonItems::ConverterVersion& converterVersion):
	 EU5Path(theConfiguration->getEU5Path())
{
	Log(LogLevel::Info) << "*** Hello EU5, loading World. ***";
	saveGame.path = theConfiguration->getEU5SaveGamePath();
	Log(LogLevel::Progress) << "6 %";

	Log(LogLevel::Info) << "-> Verifying EU5 save.";
	verifySave();
	Log(LogLevel::Progress) << "7 %";

	Log(LogLevel::Progress) << "\t* Importing Save. *";
	auto metaData = std::istringstream(saveGame.metadata);
	auto gameState = std::istringstream(saveGame.gamestate);
	registerKeys(theConfiguration, converterVersion);
	parseStream(metaData);
	if (!saveGame.metadata.empty())
		saveGame.parsedMeta = true;
	parseStream(gameState);
	clearRegisteredKeywords();
	Log(LogLevel::Progress) << "\t* Import Complete. *";
	Log(LogLevel::Progress) << "15 %";

	// With mods loaded we can init stuff that requires them.
	modFS = commonItems::ModFilesystem(EU5Path, mods);

	Log(LogLevel::Info) << "-> Booting Loaders:";
	Log(LogLevel::Info) << "\t\tRegions";
	Log(LogLevel::Info) << "\t\tColonial Regions";
	Log(LogLevel::Info) << "\t\tTrade Companies";
	Log(LogLevel::Info) << "\t\tReligions";
	Log(LogLevel::Info) << "\t\tCultures";
	Log(LogLevel::Info) << "\t\tUnit Types";
	Log(LogLevel::Info) << "\t\tCommon Countries";
	Log(LogLevel::Info) << "\t\tLocalizations";
	Log(LogLevel::Progress) << "16 %";

	Log(LogLevel::Info) << "*** Building world ***";

	Log(LogLevel::Info) << "-> Classifying Provinces According to Aesthetic Principles";
	Log(LogLevel::Progress) << "17 %";

	Log(LogLevel::Info) << "-> Calculating Province Weights";
	Log(LogLevel::Progress) << "18 %";

	Log(LogLevel::Info) << "-> Determining Demographics";
	Log(LogLevel::Progress) << "19 %";

	Log(LogLevel::Info) << "-> Linking Provinces to Countries";
	Log(LogLevel::Progress) << "20 %";

	Log(LogLevel::Info) << "-> Updating Unit Types in Regiments";
	Log(LogLevel::Progress) << "21 %";

	Log(LogLevel::Info) << "-> Injecting Imperialism into Countries";
	Log(LogLevel::Progress) << "22 %";

	Log(LogLevel::Info) << "-> Injecting Art Deco into Countries";
	Log(LogLevel::Progress) << "23 %";

	Log(LogLevel::Info) << "-> Injecting Smokestacks into Provinces";
	Log(LogLevel::Progress) << "24 %";

	Log(LogLevel::Info) << "-> Injecting Localizations into Countries";
	Log(LogLevel::Progress) << "25 %";

	Log(LogLevel::Info) << "-> Viva la revolution?";
	Log(LogLevel::Progress) << "26 %";

	Log(LogLevel::Info) << "-> Merging Nations";
	Log(LogLevel::Progress) << "27 %";

	Log(LogLevel::Info) << "-> Cataloguing Native Fauna";
	Log(LogLevel::Info) << "-> Clasifying Invasive Fauna";
	Log(LogLevel::Progress) << "28 %";

	Log(LogLevel::Info) << "-> Dropping Dead, Empty and/or Coreless Nations";
	Log(LogLevel::Progress) << "29 %";

	Log(LogLevel::Info) << "-> Splitting Trade Companies into nations";
	Log(LogLevel::Progress) << "39 %";

	Log(LogLevel::Info) << "*** Good-bye EU4, you served us well. ***";
	Log(LogLevel::Progress) << "40 %";
}

void EU5::World::registerKeys(const std::shared_ptr<Configuration>& theConfiguration, const commonItems::ConverterVersion& converterVersion)
{
	metaPreParser.registerRegex("SAV.*", [](const std::string& unused, std::istream& theStream) {
	});
	metaPreParser.registerKeyword("meta_data", [this](std::istream& theStream) {
		metaParser.parseStream(theStream);
		saveGame.parsedMeta = true;
	});
	metaPreParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	metaParser.registerKeyword("date", [this](std::istream& theStream) {
		datingData.lastEU5Date = date(commonItems::getString(theStream));
		Log(LogLevel::Info) << "\t-> Last date set to: " << datingData.lastEU5Date;
	});
	metaParser.registerKeyword("playthrough_name", [this](std::istream& theStream) {
		Log(LogLevel::Info) << "\t-> Playthrough Name: " << commonItems::getString(theStream);
	});
	metaParser.registerKeyword("save_label", [this](std::istream& theStream) {
		Log(LogLevel::Info) << "\t-> Save Label: " << commonItems::getString(theStream);
	});
	metaParser.registerKeyword("version", [this, converterVersion, theConfiguration](std::istream& theStream) {
		const commonItems::singleString versionString(theStream);
		version = GameVersion(versionString.getString());
		Log(LogLevel::Info) << "\t-> Savegave version: " << version.toString();

		if (converterVersion.getMinSource() > version)
		{
			Log(LogLevel::Error) << "Converter requires a minimum save from v" << converterVersion.getMinSource().toShortString();
			throw std::runtime_error("Savegame vs converter version mismatch!");
		}
		if (!converterVersion.getMaxSource().isLargerishThan(version))
		{
			Log(LogLevel::Error) << "Converter requires a maximum save from v" << converterVersion.getMaxSource().toShortString();
			throw std::runtime_error("Savegame vs converter version mismatch!");
		}
		if (version > theConfiguration->getEU5Version())
		{
			Log(LogLevel::Error) << "Installation version: " << theConfiguration->getEU5Version()->toString()
										<< " is older than savegame version: " << version.toString() << "! This is a no-go!";
			throw std::runtime_error("Savegame vs installation version mismatch!");
		}
	});
	metaParser.registerKeyword("player_country_name", [this](std::istream& theStream) {
		Log(LogLevel::Info) << "\t-> Player Country Name: " << commonItems::getString(theStream);
	});
	metaParser.registerKeyword("compatibility", [this](std::istream& theStream) {
		compatibilityParser.parseStream(theStream);
	});
	metaParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	compatibilityParser.registerKeyword("locations", [this](std::istream& theStream) {
		Log(LogLevel::Info) << "\t-> Registering Provinces";
		const auto& locationsSrc = commonItems::stringList(theStream).getStrings();
		if (locationsSrc.empty())
		{
			throw std::runtime_error("No locations in metadata. This is unacceptable.");
		}

		int locationID = 0;
		for (const auto& location: locationsSrc)
		{
			locationID++;
			provinceManager.registerLocation(locationID, location);
		}
		Log(LogLevel::Info) << "\t<> Registered " << locationID << " locations, from " << provinceManager.getSeenLocationByID(1)->getName() << "(1) to "
								  << provinceManager.getSeenLocationByID(locationID)->getName() << "(" << locationID << ").";
	});
	compatibilityParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	registerRegex("SAV.*", [](const std::string& unused, std::istream& theStream) {
	});
	registerKeyword("metadata", [this](std::istream& theStream) {
		if (saveGame.parsedMeta)
		{
			commonItems::ignoreItem("unused", theStream);
		}
		else
		{
			metaParser.parseStream(theStream);
			saveGame.parsedMeta = true;
		}
	});
	registerKeyword("locations", [this](std::istream& theStream) {
		Log(LogLevel::Info) << "\t-> Importing Locations";
		provinceManager.loadProvinces(theStream);
		Log(LogLevel::Info) << "\t<> Imported " << provinceManager.getAllLocations().size() << " provinces.";
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void EU5::World::verifySave()
{
	const std::ifstream saveFile(saveGame.path, std::ios::in | std::ios::binary);
	std::stringstream inStream;
	inStream << saveFile.rdbuf();
	saveGame.gamestate = inStream.str();

	const auto save = rakaly::parseEu5(saveGame.gamestate);
	if (const auto& melt = save.meltMeta(); melt)
	{
		Log(LogLevel::Info) << "Meta extracted successfully.";
		melt->writeData(saveGame.metadata);
	}
	else if (save.is_binary())
	{
		Log(LogLevel::Error) << "Binary Save and NO META!";
	}

	if (save.is_binary())
	{
		Log(LogLevel::Info) << "Gamestate is binary, melting.";
		const auto& melt = save.melt();
		if (melt.has_unknown_tokens())
		{
			Log(LogLevel::Error) << "Rakaly reports errors while melting ironman save!";
		}

		melt.writeData(saveGame.gamestate);
	}
	else
	{
		Log(LogLevel::Info) << "Gamestate is textual.";
		const auto& melt = save.melt();
		melt.writeData(saveGame.gamestate);
	}

	zip_t* zip = zip_open(saveGame.path.string().c_str(), 0, 'r');
	const auto entriesCount = zip_entries_total(zip);
	if (entriesCount > 3)
		throw std::runtime_error("Unrecognized savegame structure!");

	// Always dump to disk for easier debug.
	std::ofstream metaDump("metaDump.txt");
	metaDump << saveGame.metadata;
	metaDump.close();

	std::ofstream saveDump("saveDump.txt");
	saveDump << saveGame.gamestate;
	saveDump.close();
}
