#include "RegionManager/Area.h"
#include "RegionManager/Region.h"
#include "RegionManager/SubContinent.h"
#include "gtest/gtest.h"

TEST(Mappers_SubContinentTests, blankSubContinentLoadsWithNoRegions)
{
	std::stringstream input;
	const EU5::SubContinent subContinent(input);

	EXPECT_TRUE(subContinent.getRegions().empty());
}

TEST(Mappers_SubContinentTests, RegionsCanBeLoaded)
{
	std::stringstream input;
	input << "scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm tierp heby } } }\n";
	input << "north_german_region = { brabant_area = { limburg_province = { maastricht venlo weert roermond } } }\n";
	const EU5::SubContinent subContinent(input);

	EXPECT_TRUE(subContinent.getRegions().contains("scandinavian_region"));
	EXPECT_TRUE(subContinent.getRegions().contains("north_german_region"));
}

TEST(Mappers_SubContinentTests, SubContinentCanLocateLocation)
{
	std::stringstream input;
	input << "scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm tierp heby } } }\n";
	input << "north_german_region = { brabant_area = { limburg_province = { maastricht venlo weert roermond } } }\n";
	const EU5::SubContinent subContinent(input);

	EXPECT_TRUE(subContinent.subContinentContainsLocation("roermond"));
}

TEST(Mappers_SubContinentTests, SubContinentReturnsFalseOnLocationMismatch)
{
	std::stringstream input;
	input << "scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm tierp heby } } }\n";
	input << "north_german_region = { brabant_area = { limburg_province = { maastricht venlo weert roermond } } }\n";
	const EU5::SubContinent subContinent(input);

	EXPECT_FALSE(subContinent.subContinentContainsLocation("nonsense"));
}
