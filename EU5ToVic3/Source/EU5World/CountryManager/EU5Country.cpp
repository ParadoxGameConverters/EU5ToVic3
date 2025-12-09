#include "EU5Country.h"
#include "ParserHelpers.h"
#include <ranges>

EU5::Country::Country(int theCountryID, std::istream& theStream): countryID(std::move(theCountryID))
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::Country::registerKeys()
{
	registerKeyword("country_name", [this](std::istream& theStream) {
		countryName = commonItems::getString(theStream);
	});
	registerKeyword("flag", [this](std::istream& theStream) {
		flag = commonItems::getString(theStream);
	});
	registerKeyword("type", [this](std::istream& theStream) {
		type = commonItems::getString(theStream);
	});
	registerKeyword("historical", [this](std::istream& theStream) {
		historical = commonItems::getString(theStream);
	});
	registerKeyword("country_type", [this](std::istream& theStream) {
		countryType = commonItems::getString(theStream);
	});
}
