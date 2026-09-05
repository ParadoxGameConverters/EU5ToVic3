#include "EU5Building.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

EU5::Building::Building(int theBuildingID, std::istream& theStream): buildingID(theBuildingID)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::Building::registerKeys()
{
	registerKeyword("type", [this](std::istream& theStream) {
		type = commonItems::getString(theStream);
	});
	registerKeyword("level", [this](std::istream& theStream) {
		level = commonItems::getInt(theStream);
	});
	registerKeyword("location", [this](std::istream& theStream) {
		locationID = commonItems::getInt(theStream);
	});
	registerKeyword("owner", [this](std::istream& theStream) {
		ownerEstateID = commonItems::getInt(theStream);
	});
	registerKeyword("pop", [this](std::istream& theStream) {
		popID = commonItems::getInt(theStream);
	});
	registerKeyword("employed", [this](std::istream& theStream) {
		employed = commonItems::getDouble(theStream);
	});
	registerKeyword("employment_requirement", [this](std::istream& theStream) {
		employmentRequirement = commonItems::getInt(theStream);
	});
	registerKeyword("employment_requirement_status", [this](std::istream& theStream) {
		employmentRequirementStatus = commonItems::getString(theStream);
	});
	registerKeyword("establishment_progress", [this](std::istream& theStream) {
		establishmentProgress = commonItems::getInt(theStream);
	});
	registerKeyword("last_months_profit", [this](std::istream& theStream) {
		lastMonthsProfit = commonItems::getDouble(theStream);
	});
	registerKeyword("upkeep", [this](std::istream& theStream) {
		upkeep = commonItems::getDouble(theStream);
	});
	registerKeyword("open", [this](std::istream& theStream) {
		open = commonItems::getString(theStream) == "yes";
	});
	registerKeyword("subsidized", [this](std::istream& theStream) {
		subsidized = commonItems::getString(theStream) == "yes";
	});
	registerRegex(commonItems::catchallRegex, [this](const std::string& productionMethodKey, std::istream& theStream) {
		productionMethod = productionMethodKey;
		parser parser;
		parser.registerKeyword("input", [this](std::istream& theStream) {
			input = commonItems::getDouble(theStream);
		});
		parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
		parser.parseStream(theStream);
	});
}
