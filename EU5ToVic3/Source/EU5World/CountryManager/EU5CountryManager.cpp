#include "EU5CountryManager.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"
#include <iomanip>
#include <ranges>

void EU5::CountryManager::loadCountries(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();

	if (revolutionaryTarget > 0 && tagRegistry.contains(revolutionaryTarget))
	{
		if (auto theTarget = getCountryByID(revolutionaryTarget); theTarget)
		{

			theTarget->setRevolutionaryTarget();
			Log(LogLevel::Info) << "\t^^^ Revolution Lives! (" << theTarget->getTag() << ")";

			return;
		}
	}
	Log(LogLevel::Info) << "\tvvv ... revolution failed. :/";
}

void EU5::CountryManager::registerKeys()
{
	tagRegistryParser.registerRegex(commonItems::integerRegex, [this](const std::string& theID, std::istream& theStream) {
		auto tag = commonItems::getString(theStream);
		if (tag != "---" && tag != "DUMMY" && tag != "PIR" && tag != "MER") // Ignore ---, dummy, pirates and mercenaries.
		{
			tagRegistry.emplace(std::stoi(theID), tag);
		}
	});
	tagRegistryParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	registerKeyword("tags", [this](const std::string& unused, std::istream& theStream) {
		tagRegistryParser.parseStream(theStream);
		Log(LogLevel::Info) << "\t-> Registered " << tagRegistry.size() << " tags.";
	});

	countryDatabaseParser.registerRegex(commonItems::integerRegex, [this](const std::string& theID, std::istream& theStream) {
		const auto countryID = std::stoi(theID);
		if (tagRegistry.contains(countryID))
		{
			const auto& tag = tagRegistry.at(countryID);
			auto newCountry = std::make_shared<Country>(countryID, theStream);
			newCountry->setTag(tag);
			countries.emplace(tag, newCountry);
		}
		else
		{
			commonItems::ignoreItem("unused", theStream); // Ignore all dead dereferenced countries + pirates, mercs etc.
		}
	});
	countryDatabaseParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	registerKeyword("database", [this](const std::string& unused, std::istream& theStream) {
		countryDatabaseParser.parseStream(theStream);
	});

	registerKeyword("revolutionary_target", [this](const std::string& unused, std::istream& theStream) {
		revolutionaryTarget = commonItems::getInt(theStream);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::shared_ptr<EU5::Country> EU5::CountryManager::getCountryByID(int countryID) const
{
	for (const auto& country: countries | std::views::values)
	{
		if (country->getID() == countryID)
			return country;
	}
	return nullptr;
}
