#include "EU5Culture.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

EU5::Culture::Culture(int theCultureID, std::istream& theStream): cultureID(theCultureID)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::Culture::registerKeys()
{
	registerKeyword("name", [this](std::istream& theStream) {
		name = commonItems::getString(theStream);
	});
	registerKeyword("culture_definition", [this](std::istream& theStream) {
		cultureDefinition = commonItems::getString(theStream);
	});
	registerKeyword("color", [this](std::istream& theStream) {
		color = commonItems::Color::Factory{}.getColor(theStream);
	});
	registerKeyword("language", [this](std::istream& theStream) {
		language = commonItems::getString(theStream);
	});
	registerKeyword("size", [this](std::istream& theStream) {
		size = commonItems::getDouble(theStream);
	});
	registerKeyword("cultural_influence", [this](std::istream& theStream) {
		culturalInfluence = commonItems::getDouble(theStream);
	});
	registerKeyword("cultural_tradition", [this](std::istream& theStream) {
		culturalTradition = commonItems::getDouble(theStream);
	});
	registerKeyword("active", [this](std::istream& theStream) {
		active = commonItems::getString(theStream) == "yes";
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
