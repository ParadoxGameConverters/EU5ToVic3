#include "SubContinent.h"
#include <ranges>

EU5::SubContinent::SubContinent(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

bool EU5::SubContinent::subContinentContainsLocation(const std::string& location) const
{
	for (const auto& region: regions | std::views::values)
		if (region->regionContainsLocation(location))
			return true;
	return false;
}

void EU5::SubContinent::registerKeys()
{
	registerRegex(R"([\w_]+)", [this](const std::string& regionName, std::istream& theStream) {
		regions.emplace(regionName, std::make_shared<Region>(theStream));
	});
}
