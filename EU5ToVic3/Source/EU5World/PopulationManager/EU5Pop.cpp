#include "EU5Pop.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

EU5::Pop::Pop(int thePopID, std::istream& theStream): popID(thePopID)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::Pop::registerKeys()
{
	registerKeyword("type", [this](std::istream& theStream) {
		type = commonItems::getString(theStream);
	});
	registerKeyword("estate", [this](std::istream& theStream) {
		estate = commonItems::getString(theStream);
	});
	registerKeyword("culture", [this](std::istream& theStream) {
		cultureID = commonItems::getInt(theStream);
	});
	registerKeyword("status", [this](std::istream& theStream) {
		status = commonItems::getString(theStream);
	});
	registerKeyword("religion", [this](std::istream& theStream) {
		religionID = commonItems::getInt(theStream);
	});
	registerKeyword("satisfaction", [this](std::istream& theStream) {
		satisfaction = commonItems::getDouble(theStream);
	});
	registerKeyword("size", [this](std::istream& theStream) {
		size = commonItems::getDouble(theStream);
	});
	registerKeyword("literacy", [this](std::istream& theStream) {
		literacy = commonItems::getDouble(theStream);
	});

	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}