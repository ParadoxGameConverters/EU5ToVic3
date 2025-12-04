#include "Province.h"

EU5::Province::Province(const std::vector<std::string>& theLocations)
{
	for (const auto& location: theLocations)
		locations.emplace(location);
}

bool EU5::Province::provinceContainsLocation(const std::string& location) const
{
	return locations.contains(location);
}
