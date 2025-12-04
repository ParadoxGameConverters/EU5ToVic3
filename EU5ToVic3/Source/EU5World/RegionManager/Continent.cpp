#include "Continent.h"
#include <ranges>

EU5::Continent::Continent(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

bool EU5::Continent::continentContainsLocation(const std::string& location) const
{
	for (const auto& superRegion: superRegions | std::views::values)
		if (superRegion->superRegionContainsLocation(location))
			return true;
	return false;
}

void EU5::Continent::registerKeys()
{
	registerRegex(R"([\w_]+)", [this](const std::string& superRegionName, std::istream& theStream) {
		superRegions.emplace(superRegionName, std::make_shared<SuperRegion>(theStream));
	});
}