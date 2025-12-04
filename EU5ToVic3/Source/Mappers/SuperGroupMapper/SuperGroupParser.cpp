#include "SuperGroupParser.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::SuperGroupParser::SuperGroupParser(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::SuperGroupParser::registerKeys()
{
	registerKeyword("continents", [this](const std::string& unused, std::istream& theStream) {
		Log(LogLevel::Debug) << "un: " << unused;
		const auto& continentList = commonItems::stringList(theStream).getStrings();
		for (const auto& continent: continentList)
		{
			continentNames.emplace(continent);
			Log(LogLevel::Debug) << "sn: " << continent;
		}
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
