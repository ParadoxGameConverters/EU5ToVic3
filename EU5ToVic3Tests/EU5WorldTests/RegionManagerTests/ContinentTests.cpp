#include "RegionManager/Area.h"
#include "RegionManager/Region.h"
#include "RegionManager/SuperRegion.h"
#include "RegionManager/Continent.h"
#include "gtest/gtest.h"

TEST(Mappers_ContinentTests, blankContinentLoadsWithNoSuperRegions)
{
	std::stringstream input;
	const EU5::Continent continent(input);

	EXPECT_TRUE(continent.getSuperRegions().empty());
}

TEST(Mappers_ContinentTests, SuperRegionsCanBeLoaded)
{
	std::stringstream input;
	input << "western_europe = { scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm tierp heby } } } }\n";
	input << "eastern_europe = { carpathia_region = { moldavia_area = { bacau_province = { bacau hangu_romania piatra_lui_craciun roman targu_neamt targu_trotus } } } }\n";
	const EU5::Continent continent(input);

	EXPECT_TRUE(continent.getSuperRegions().contains("western_europe"));
	EXPECT_TRUE(continent.getSuperRegions().contains("eastern_europe"));
}

TEST(Mappers_ContinentTests, ContinentCanLocateLocation)
{
	std::stringstream input;
	input << "western_europe = { scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm tierp heby } } "
				"} }\n";
	input << "eastern_europe = { carpathia_region = { moldavia_area = { bacau_province = { bacau hangu_romania piatra_lui_craciun roman targu_neamt "
				"targu_trotus } } } }\n";
	const EU5::Continent continent(input);

	EXPECT_TRUE(continent.continentContainsLocation("roman"));
}

TEST(Mappers_ContinentTests, ContinentReturnsFalseOnLocationMismatch)
{
	std::stringstream input;
	input << "western_europe = { scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm tierp heby } } "
				"} }\n";
	input << "eastern_europe = { carpathia_region = { moldavia_area = { bacau_province = { bacau hangu_romania piatra_lui_craciun roman targu_neamt "
				"targu_trotus } } } }\n";
	const EU5::Continent continent(input);

	EXPECT_FALSE(continent.continentContainsLocation("nonsense"));
}
