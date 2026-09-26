#include "BuildingManager.h"
#include "CommonRegexes.h"

void EU5::BuildingManager::loadBuildings(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::BuildingManager::registerKeys()
{
	buildingDatabaseParser.registerRegex(commonItems::integerRegex, [this](const std::string& theID, std::istream& theStream) {
		commonItems::parser::getNextTokenWithoutMatching(theStream); // remove equals
		theStream >> std::ws;
		if (theStream.peek() != '{')
		{
			commonItems::parser::getNextTokenWithoutMatching(theStream); // consume the "none" sentinel
			return;
		}
		const auto newBuildingID = std::stoi(theID);
		const auto& newBuilding = std::make_shared<Building>(newBuildingID, theStream);
		buildings.emplace(newBuildingID, newBuilding);
		buildingIDsByLocation[newBuilding->getLocationID()].push_back(newBuildingID);
	});
	buildingDatabaseParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);

	registerKeyword("database", [this](const std::string& unused, std::istream& theStream) {
		buildingDatabaseParser.parseStream(theStream);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

std::shared_ptr<EU5::Building> EU5::BuildingManager::getBuildingByID(int theBuildingID) const
{
	if (const auto& itr = buildings.find(theBuildingID); itr != buildings.end())
		return itr->second;
	return nullptr;
}

std::vector<int> EU5::BuildingManager::getBuildingIDsAtLocation(int theLocationID) const
{
	if (const auto& itr = buildingIDsByLocation.find(theLocationID); itr != buildingIDsByLocation.end())
		return itr->second;
	return {};
}
