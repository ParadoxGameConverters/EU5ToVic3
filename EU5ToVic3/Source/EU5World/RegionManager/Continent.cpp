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
	for (const auto& subContinent: subContinents | std::views::values)
		if (subContinent->subContinentContainsLocation(location))
			return true;
	return false;
}

void EU5::Continent::registerKeys()
{
	registerRegex(R"([\w_]+)", [this](const std::string& subContinentName, std::istream& theStream) {
		subContinents.emplace(subContinentName, std::make_shared<SubContinent>(theStream));
	});
}
