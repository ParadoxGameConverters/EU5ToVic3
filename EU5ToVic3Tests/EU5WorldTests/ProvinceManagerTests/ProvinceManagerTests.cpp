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

TEST(EU5World_ProvinceManagerTests, accessingNonExistentLocationThrowsException)
{
	EU5::ProvinceManager theProvinceManager;
	theProvinceManager.registerLocation(1, "a");
	theProvinceManager.registerLocation(2, "b");

	EXPECT_THROW(auto a = theProvinceManager.getSeenLocationByID(42), std::runtime_error);
}

TEST(EU5World_ProvinceManagerTests, locationsCanBeRetrieved)
{
	EU5::ProvinceManager theProvinceManager;
	theProvinceManager.registerLocation(1, "a");
	theProvinceManager.registerLocation(2, "b");

	EXPECT_EQ(1, theProvinceManager.getSeenLocationByID(1)->getID());
	EXPECT_EQ(2, theProvinceManager.getSeenLocationByID(2)->getID());
}
