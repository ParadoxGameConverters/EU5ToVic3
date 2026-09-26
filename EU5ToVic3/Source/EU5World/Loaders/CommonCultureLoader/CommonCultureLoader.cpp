#include "CommonCultureLoader.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

void EU5::CommonCultureLoader::loadCommonCultures(const commonItems::ModFilesystem& modFS)
{
	registerKeys();
	for (const auto& file: modFS.GetAllFilesInFolder("in_game/common/cultures"))
	{
		if (file.extension() != ".txt")
			continue;
		parseFile(file);
	}
	clearRegisteredKeywords();
}

void EU5::CommonCultureLoader::loadCommonCultures(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::CommonCultureLoader::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& cultureName, std::istream& theStream) {
		parser cultureParser;
		cultureParser.registerKeyword("language", [this, &cultureName](std::istream& languageStream) {
			cultureLanguages.emplace(cultureName, commonItems::getString(languageStream));
		});
		cultureParser.registerKeyword("color", [this, &cultureName](std::istream& colorStream) {
			cultureColors.emplace(cultureName, commonItems::getString(colorStream));
		});
		cultureParser.registerKeyword("culture_groups", [this, &cultureName](std::istream& groupsStream) {
			cultureGroups.emplace(cultureName, commonItems::stringList(groupsStream).getStrings());
		});
		cultureParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
		cultureParser.parseStream(theStream);
	});
}

std::optional<std::string> EU5::CommonCultureLoader::getLanguage(const std::string& cultureName) const
{
	if (const auto& itr = cultureLanguages.find(cultureName); itr != cultureLanguages.end())
		return itr->second;
	return std::nullopt;
}

std::optional<std::string> EU5::CommonCultureLoader::getColor(const std::string& cultureName) const
{
	if (const auto& itr = cultureColors.find(cultureName); itr != cultureColors.end())
		return itr->second;
	return std::nullopt;
}

std::optional<std::vector<std::string>> EU5::CommonCultureLoader::getCultureGroups(const std::string& cultureName) const
{
	if (const auto& itr = cultureGroups.find(cultureName); itr != cultureGroups.end())
		return itr->second;
	return std::nullopt;
}
