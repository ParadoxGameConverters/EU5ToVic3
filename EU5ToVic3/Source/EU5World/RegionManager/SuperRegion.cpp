#include "SuperRegion.h"
#include <ranges>


EU5::SuperRegion::SuperRegion(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

bool EU5::SuperRegion::superRegionContainsLocation(const std::string& location) const
{
	for (const auto& region: regions | std::views::values)
		if (region->regionContainsLocation(location))
			return true;
	return false;
}

void EU5::SuperRegion::registerKeys()
{
	registerRegex(R"([\w_]+)", [this](const std::string& regionName, std::istream& theStream) {
		regions.emplace(regionName, std::make_shared<Region>(theStream));
	});
}
