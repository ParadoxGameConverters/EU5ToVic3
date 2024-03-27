#include "ModNames.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

EU5::ModNames::ModNames(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::ModNames::registerKeys()
{
	registerSetter("filename", path);
	registerSetter("name", name);
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
