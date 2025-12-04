#include "Region.h"
#include <ranges>

EU5::Region::Region(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::Region::registerKeys()
{
	registerRegex(R"([\w_]+)", [this](const std::string& areaName, std::istream& theStream) {
		areas.emplace(areaName, std::make_shared<Area>(theStream));
	});
}

bool EU5::Region::regionContainsLocation(const std::string& theLocation) const
{
	for (const auto& area: areas | std::views::values)
		if (area->areaContainsLocation(theLocation))
			return true;
	return false;
}
