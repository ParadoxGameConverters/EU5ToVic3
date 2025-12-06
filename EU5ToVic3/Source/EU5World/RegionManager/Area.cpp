#include "Area.h"
#include <ranges>

EU5::Area::Area(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::Area::registerKeys()
{
	registerRegex(R"([\w_]+)", [this](const std::string& provinceName, std::istream& theStream) {
		auto locations = commonItems::stringList(theStream);
		auto newProvince = std::make_shared<Province>(locations.getStrings());
		provinces.emplace(provinceName, newProvince);
	});
}

bool EU5::Area::areaContainsLocation(const std::string& theLocation) const
{
	for (const auto& province: provinces | std::views::values)
		if (province->provinceContainsLocation(theLocation))
			return true;
	return false;
}
