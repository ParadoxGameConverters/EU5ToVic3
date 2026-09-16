#include "CultureManager.h"
#include "CommonRegexes.h"

void EU5::CultureManager::loadCultures(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::CultureManager::registerKeys()
{
	cultureDatabaseParser.registerRegex(commonItems::integerRegex, [this](const std::string& theID, std::istream& theStream) {
		const auto newCultureID = std::stoi(theID);
		cultures.emplace(newCultureID, std::make_shared<Culture>(newCultureID, theStream));
	});
	cultureDatabaseParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	registerKeyword("database", [this](const std::string& unused, std::istream& theStream) {
		cultureDatabaseParser.parseStream(theStream);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::shared_ptr<EU5::Culture> EU5::CultureManager::getCultureByID(int theCultureID) const
{
	if (const auto& itr = cultures.find(theCultureID); itr != cultures.end())
		return itr->second;
	return nullptr;
}
