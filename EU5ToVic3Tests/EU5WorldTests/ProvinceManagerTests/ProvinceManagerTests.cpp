#include "ProvinceManager/ProvinceManager.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>

TEST(EU5World_ProvinceManagerTests, defaultProvinceManagerIsEmpty)
{
	EU5::ProvinceManager theProvinceManager;

	EXPECT_TRUE(theProvinceManager.getSeenLocations().empty());
}

TEST(EU5World_ProvinceManagerTests, LocationsCanBeRegistered)
{
	EU5::ProvinceManager theProvinceManager;
	theProvinceManager.registerLocation(1, "a");
	theProvinceManager.registerLocation(2, "b");

	EXPECT_TRUE(theProvinceManager.getSeenLocations().contains("a"));
	EXPECT_TRUE(theProvinceManager.getSeenLocations().contains("b"));
}

TEST(EU5World_ProvinceManagerTests, accessingNonExistentLocationReturnsNullptr)
{
	EU5::ProvinceManager theProvinceManager;
	theProvinceManager.registerLocation(1, "a");
	theProvinceManager.registerLocation(2, "b");

	auto location = theProvinceManager.getSeenLocationByID(42);
	EXPECT_EQ(nullptr, location);
}

TEST(EU5World_ProvinceManagerTests, SameNamedLocationsCanNotBeRegistered)
{
	EU5::ProvinceManager theProvinceManager;
	theProvinceManager.registerLocation(1, "a");
	theProvinceManager.registerLocation(2, "a");

	EXPECT_TRUE(theProvinceManager.getSeenLocations().contains("a"));
	EXPECT_EQ(1, theProvinceManager.getSeenLocationByID(1)->getID());

	auto two = theProvinceManager.getSeenLocationByID(2);
	EXPECT_EQ(nullptr, two);
}

TEST(EU5World_ProvinceManagerTests, locationsCanBeRetrieved)
{
	EU5::ProvinceManager theProvinceManager;
	theProvinceManager.registerLocation(1, "a");
	theProvinceManager.registerLocation(2, "b");

	EXPECT_EQ(1, theProvinceManager.getSeenLocationByID(1)->getID());
	EXPECT_EQ(2, theProvinceManager.getSeenLocationByID(2)->getID());
}
