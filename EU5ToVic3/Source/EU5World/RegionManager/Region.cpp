#include "Region.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"
#include <ranges>

EU5::Region::Region(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::Region::registerKeys()
{
	registerKeyword("areas", [this](std::istream& theStream) {
		for (const auto& name: commonItems::getStrings(theStream))
			areas.emplace(name, nullptr);
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

bool EU5::Region::regionContainsProvince(int provinceID) const
{
	for (const auto& area: areas | std::views::values)
		if (area->areaContainsProvince(provinceID))
			return true;
	return false;
}
