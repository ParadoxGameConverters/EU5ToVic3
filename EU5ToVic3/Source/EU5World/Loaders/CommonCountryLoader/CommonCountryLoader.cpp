#include "CommonCountryLoader.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"
#include <filesystem>
#include <fstream>
#include <ranges>
namespace fs = std::filesystem;

void EU5::CommonCountryLoader::loadCommonCountries(const commonItems::ModFilesystem& modFS)
{
	registerKeys();

	// scoop up all country definitions
	for (const auto& file: modFS.GetAllFilesInFolder("in_game/setup/countries"))
	{
		if (file.extension() != ".txt")
			continue;
		parseFile(file);
	}

	Log(LogLevel::Info) << "<> " << countryColors1.size() << " common countries processed.";
	clearRegisteredKeywords();
}

void EU5::CommonCountryLoader::loadCommonCountries(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::CommonCountryLoader::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& tag, std::istream& theStream) {
		currentTag = tag;
		tagParser.parseStream(theStream);
	});

	tagParser.registerKeyword("color", [this](std::istream& theStream) {
		auto color = commonItems::Color::Factory{}.getColor(theStream);
		countryColors1.emplace(currentTag, color);
	});
	tagParser.registerKeyword("color2", [this](std::istream& theStream) {
		auto color = commonItems::Color::Factory{}.getColor(theStream);
		countryColors2.emplace(currentTag, color);
	});
	tagParser.registerKeyword("culture_definition", [this](std::istream& theStream) {
		countryCulture.emplace(currentTag, commonItems::getString(theStream));
	});
	tagParser.registerKeyword("religion_definition", [this](std::istream& theStream) {
		countryReligion.emplace(currentTag, commonItems::getString(theStream));
	});
	tagParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::optional<commonItems::Color> EU5::CommonCountryLoader::getColor1(const std::string& tag) const
{
	if (const auto& colorItr = countryColors1.find(tag); colorItr != countryColors1.end())
		return colorItr->second;
	return std::nullopt;
}

std::optional<commonItems::Color> EU5::CommonCountryLoader::getColor2(const std::string& tag) const
{
	if (const auto& colorItr = countryColors2.find(tag); colorItr != countryColors2.end())
		return colorItr->second;
	return std::nullopt;
}

std::optional<std::string> EU5::CommonCountryLoader::getCountryCulture(const std::string& tag) const
{
	if (const auto& cultureItr = countryCulture.find(tag); cultureItr != countryCulture.end())
		return cultureItr->second;
	return std::nullopt;
}

std::optional<std::string> EU5::CommonCountryLoader::getCountryReligion(const std::string& tag) const
{
	if (const auto& religionItr = countryReligion.find(tag); religionItr != countryReligion.end())
		return religionItr->second;
	return std::nullopt;
}
