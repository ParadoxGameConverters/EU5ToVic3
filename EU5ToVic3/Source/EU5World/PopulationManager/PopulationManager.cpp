#include "PopulationManager.h"
#include "CommonRegexes.h"

void EU5::PopulationManager::loadPopulation(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::PopulationManager::registerKeys()
{
	populationDatabaseParser.registerRegex(commonItems::integerRegex, [this](const std::string& theID, std::istream& theStream) {
		const auto newPopID = std::stoi(theID);
		pops.emplace(newPopID, std::make_shared<Pop>(newPopID, theStream));
	});
	populationDatabaseParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	registerKeyword("database", [this](const std::string& unused, std::istream& theStream) {
		populationDatabaseParser.parseStream(theStream);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::shared_ptr<EU5::Pop> EU5::PopulationManager::getPopByID(int thePopID) const
{
	if (const auto& itr = pops.find(thePopID); itr != pops.end())
		return itr->second;
	return nullptr;
}
