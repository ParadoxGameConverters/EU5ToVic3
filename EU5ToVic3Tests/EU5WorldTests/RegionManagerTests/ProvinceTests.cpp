#include "RegionManager/Province.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>

TEST(Mappers_ProvinceTests, blankProvincesLoadForNoLocations)
{
	std::vector<std::string> locations;
	const EU5::Province newProvince(locations);

	EXPECT_TRUE(newProvince.getLocations().empty());
}

TEST(Mappers_ProvinceTests, locationsCanBeLoaded)
{
	std::vector<std::string> locations = {"a", "b", "c"};
	const EU5::Province newProvince(locations);

	EXPECT_THAT(newProvince.getLocations(), testing::UnorderedElementsAre("a", "b", "c"));
}

TEST(Mappers_ProvinceTests, locationsCanBeFound)
{
	std::vector<std::string> locations = {"a", "b", "c"};
	const EU5::Province newProvince(locations);

	EXPECT_TRUE(newProvince.provinceContainsLocation("a"));
	EXPECT_TRUE(newProvince.provinceContainsLocation("b"));
	EXPECT_TRUE(newProvince.provinceContainsLocation("c"));
}

TEST(Mappers_ProvinceTests, provinceMismatchReturnsFalse)
{
	std::vector<std::string> locations = {"a", "b", "c"};
	const EU5::Province newProvince(locations);

	EXPECT_FALSE(newProvince.provinceContainsLocation("d"));
}
