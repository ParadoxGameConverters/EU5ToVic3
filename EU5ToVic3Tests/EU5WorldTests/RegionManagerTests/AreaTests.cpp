#include "RegionManager/Area.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>

TEST(Mappers_AreaTests, blankAreasLoadForNoProvinces)
{
	std::stringstream input;
	const EU5::Area newArea(input);

	EXPECT_TRUE(newArea.getProvinces().empty());
}

TEST(Mappers_AreaTests, ProvincesCanBeLoaded)
{
	std::stringstream input;
	input << "uppland_province = { stockholm norrtalje enkoping uppsala kastelholm tierp heby }\n";
	input << "sodermanland_province = { nykoping kolmarden strangnas oppunda sodertalje malaren hjalmaren_lake }\n";
	const EU5::Area newArea(input);

	EXPECT_TRUE(newArea.getProvinces().contains("uppland_province"));
	EXPECT_TRUE(newArea.getProvinces().contains("sodermanland_province"));
}

TEST(Mappers_AreaTests, LocationsCanBeFound)
{
	std::stringstream input;
	input << "uppland_province = { stockholm norrtalje enkoping uppsala kastelholm tierp heby }\n";
	input << "sodermanland_province = { nykoping kolmarden strangnas oppunda sodertalje malaren hjalmaren_lake }\n";
	const EU5::Area newArea(input);

	EXPECT_TRUE(newArea.areaContainsLocation("kolmarden"));
	EXPECT_TRUE(newArea.areaContainsLocation("malaren"));
	EXPECT_TRUE(newArea.areaContainsLocation("uppsala"));
}

TEST(Mappers_AreaTests, LocationMismatchReturnsFalse)
{
	std::stringstream input;
	input << "uppland_province = { stockholm norrtalje enkoping uppsala kastelholm tierp heby }\n";
	input << "sodermanland_province = { nykoping kolmarden strangnas oppunda sodertalje malaren hjalmaren_lake }\n";
	const EU5::Area newArea(input);

	EXPECT_FALSE(newArea.areaContainsLocation("nonsense"));
}
