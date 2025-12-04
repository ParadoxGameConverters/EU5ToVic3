#include "RegionManager/Area.h"
#include "RegionManager/Region.h"
#include "RegionManager/SuperRegion.h"
#include "gtest/gtest.h"

TEST(Mappers_SuperRegionTests, blankSuperRegionLoadsWithNoRegions)
{
	std::stringstream input;
	const EU5::SuperRegion superRegion(input);

	EXPECT_TRUE(superRegion.getRegions().empty());
}

TEST(Mappers_SuperRegionTests, RegionsCanBeLoaded)
{
	std::stringstream input;
	input << "scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm tierp heby } } }\n";
	input << "north_german_region = { brabant_area = { limburg_province = { maastricht venlo weert roermond } } }\n";
	const EU5::SuperRegion superRegion(input);

	EXPECT_TRUE(superRegion.getRegions().contains("scandinavian_region"));
	EXPECT_TRUE(superRegion.getRegions().contains("north_german_region"));
}

TEST(Mappers_SuperRegionTests, SuperRegionCanLocateLocation)
{
	std::stringstream input;
	input << "scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm tierp heby } } }\n";
	input << "north_german_region = { brabant_area = { limburg_province = { maastricht venlo weert roermond } } }\n";
	const EU5::SuperRegion superRegion(input);

	EXPECT_TRUE(superRegion.superRegionContainsLocation("roermond"));
}

TEST(Mappers_SuperRegionTests, SuperRegionReturnsFalseOnLocationMismatch)
{
	std::stringstream input;
	input << "scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm tierp heby } } }\n";
	input << "north_german_region = { brabant_area = { limburg_province = { maastricht venlo weert roermond } } }\n";
	const EU5::SuperRegion superRegion(input);

	EXPECT_FALSE(superRegion.superRegionContainsLocation("nonsense"));
}
