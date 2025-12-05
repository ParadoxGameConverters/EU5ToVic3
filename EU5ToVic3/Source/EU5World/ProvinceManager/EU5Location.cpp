#include "EU5Location.h"

EU5::Location::Location(int theLocationID, std::string theLocationName): locID(theLocationID), name(std::move(theLocationName))
{
}

void EU5::Location::parseData(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::Location::registerKeys()
{
	registerKeyword("owner", [this](std::istream& theStream) {
		ownerID = commonItems::getInt(theStream);
	});
	registerKeyword("controller", [this](std::istream& theStream) {
		controllerID = commonItems::getInt(theStream);
	});
	registerKeyword("development", [this](std::istream& theStream) {
		development = commonItems::getDouble(theStream);
	});
	registerKeyword("language", [this](std::istream& theStream) {
		language = commonItems::getString(theStream);
	});
	registerKeyword("dialect", [this](std::istream& theStream) {
		dialect = commonItems::getString(theStream);
	});
	registerKeyword("rank", [this](std::istream& theStream) {
		rank = commonItems::getString(theStream);
	});
	registerKeyword("cores", [this](std::istream& theStream) {
		const auto& coreList = commonItems::getInts(theStream);
		coreIDs.insert(coreList.begin(), coreList.end());
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
