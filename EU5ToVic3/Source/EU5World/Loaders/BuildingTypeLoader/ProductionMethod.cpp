#include "ProductionMethod.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

EU5::ProductionMethod::ProductionMethod(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::ProductionMethod::registerKeys()
{
	registerKeyword("produced", [this](std::istream& theStream) {
		producedGood = commonItems::getString(theStream);
	});
	registerKeyword("output", commonItems::ignoreItem);
	registerKeyword("debug_max_profit", commonItems::ignoreItem);
	registerKeyword("category", commonItems::ignoreItem);
	registerRegex(commonItems::catchallRegex, [this](const std::string& goodName, std::istream& theStream) {
		inputGoods.push_back(goodName);
		commonItems::ignoreItem(goodName, theStream);
	});
}
