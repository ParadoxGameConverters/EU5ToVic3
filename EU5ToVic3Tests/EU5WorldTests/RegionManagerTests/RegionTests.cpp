#include "RegionManager/Area.h"
#include "RegionManager/Region.h"
#include "gtest/gtest.h"

TEST(Mappers_RegionTests, blankRegionLoadsWithNoAreas)
{
	std::stringstream input;
	const EU5::Region region(input);

	EXPECT_TRUE(region.getAreas().empty());
}

TEST(Mappers_RegionTests, areasCanBeLoaded)
{
	std::stringstream input;
	input << "svealand_area = { uppland_province = {stockholm norrtalje enkoping uppsala kastelholm tierp heby} }\n";
	input << "gotaland_area = { ostergotland_province = {linkoping soderkoping norrkoping alvastra vadstena vattern} }\n";
	const EU5::Region region(input);

	EXPECT_TRUE(region.getAreas().contains("svealand_area"));
	EXPECT_TRUE(region.getAreas().contains("gotaland_area"));
}

TEST(Mappers_RegionTests, RegionCanLocateLocation)
{
	std::stringstream input;
	input << "svealand_area = { uppland_province = {stockholm norrtalje enkoping uppsala kastelholm tierp heby} }\n";
	input << "gotaland_area = { ostergotland_province = {linkoping soderkoping norrkoping alvastra vadstena vattern} }\n";
	const EU5::Region region(input);

	EXPECT_TRUE(region.regionContainsLocation("stockholm"));
}

TEST(Mappers_RegionTests, RegionReturnsFalseForLocationMismatch)
{
	std::stringstream input;
	input << "svealand_area = { uppland_province = {stockholm norrtalje enkoping uppsala kastelholm tierp heby} }\n";
	input << "gotaland_area = { ostergotland_province = {linkoping soderkoping norrkoping alvastra vadstena vattern} }\n";
	const EU5::Region region(input);

	EXPECT_FALSE(region.regionContainsLocation("nonsense"));
}
