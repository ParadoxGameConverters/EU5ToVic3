#include "ProvinceManager/ProvinceManager.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>

TEST(EU5_ProvinceManagerTests, defaultProvinceManagerIsEmpty)
{
	EU5::ProvinceManager theProvinceManager;

	EXPECT_TRUE(theProvinceManager.getSeenLocations().empty());
}

TEST(EU5_ProvinceManagerTests, LocationsCanBeRegistered)
{
	EU5::ProvinceManager theProvinceManager;
	theProvinceManager.registerLocation(1, "a");
	theProvinceManager.registerLocation(2, "b");

	EXPECT_TRUE(theProvinceManager.getSeenLocations().contains("a"));
	EXPECT_TRUE(theProvinceManager.getSeenLocations().contains("b"));
}

TEST(EU5_ProvinceManagerTests, accessingNonExistentLocationReturnsNullptr)
{
	EU5::ProvinceManager theProvinceManager;
	theProvinceManager.registerLocation(1, "a");
	theProvinceManager.registerLocation(2, "b");

	auto location = theProvinceManager.getSeenLocationByID(42);
	EXPECT_EQ(nullptr, location);
}

TEST(EU5_ProvinceManagerTests, SameNamedLocationsCanNotBeRegistered)
{
	EU5::ProvinceManager theProvinceManager;
	theProvinceManager.registerLocation(1, "a");
	theProvinceManager.registerLocation(2, "a");

	EXPECT_TRUE(theProvinceManager.getSeenLocations().contains("a"));
	EXPECT_EQ(1, theProvinceManager.getSeenLocationByID(1)->getID());

	auto two = theProvinceManager.getSeenLocationByID(2);
	EXPECT_EQ(nullptr, two);
}

TEST(EU5_ProvinceManagerTests, LocationsCanBeRetrieved)
{
	EU5::ProvinceManager theProvinceManager;
	theProvinceManager.registerLocation(1, "a");
	theProvinceManager.registerLocation(2, "b");

	EXPECT_EQ(1, theProvinceManager.getSeenLocationByID(1)->getID());
	EXPECT_EQ(2, theProvinceManager.getSeenLocationByID(2)->getID());
}

TEST(EU5_ProvinceManagerTests, LocationsCanBeParsed)
{
	EU5::ProvinceManager theProvinceManager;
	theProvinceManager.registerLocation(1, "a");
	theProvinceManager.registerLocation(2, "b");

	std::stringstream input;
	input << "locations={\n";
	input << "1 = { owner = 4 }\n";
	input << "2 = { controller = 5 }\n";
	input << "}\n";
	theProvinceManager.loadProvinces(input);

	EXPECT_EQ(4, theProvinceManager.getAllLocations().at("a")->getOwnerID());
	EXPECT_EQ(5, theProvinceManager.getAllLocations().at("b")->getControllerID());
}

TEST(EU5_ProvinceManagerTests, UnregisteredLocationsCannotBeParsed)
{
	EU5::ProvinceManager theProvinceManager;
	std::stringstream input;
	input << "locations={\n";
	input << "1 = { owner = 4 }\n";
	input << "2 = { controller = 5 }\n";
	input << "}\n";
	theProvinceManager.loadProvinces(input);

	EXPECT_TRUE(theProvinceManager.getAllLocations().empty());
}
