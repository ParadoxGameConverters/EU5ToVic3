#include "LocationManager/LocationManager.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>

TEST(EU5World_LocationManagerTests, defaultLocationManagerIsEmpty)
{
	EU5::LocationManager theLocationManager;

	EXPECT_TRUE(theLocationManager.getSeenLocations().empty());
}

TEST(EU5World_LocationManagerTests, LocationsCanBeRegistered)
{
	EU5::LocationManager theLocationManager;
	theLocationManager.registerLocation(1, "a");
	theLocationManager.registerLocation(2, "b");

	EXPECT_TRUE(theLocationManager.getSeenLocations().contains("a"));
	EXPECT_TRUE(theLocationManager.getSeenLocations().contains("b"));
}

TEST(EU5World_LocationManagerTests, accessingNonExistentLocationReturnsNullptr)
{
	EU5::LocationManager theLocationManager;
	theLocationManager.registerLocation(1, "a");
	theLocationManager.registerLocation(2, "b");

	auto location = theLocationManager.getSeenLocationByID(42);
	EXPECT_EQ(nullptr, location);
}

TEST(EU5World_LocationManagerTests, SameNamedLocationsCanNotBeRegistered)
{
	EU5::LocationManager theLocationManager;
	theLocationManager.registerLocation(1, "a");
	theLocationManager.registerLocation(2, "a");

	EXPECT_TRUE(theLocationManager.getSeenLocations().contains("a"));
	EXPECT_EQ(1, theLocationManager.getSeenLocationByID(1)->getID());

	auto two = theLocationManager.getSeenLocationByID(2);
	EXPECT_EQ(nullptr, two);
}

TEST(EU5World_LocationManagerTests, LocationsCanBeRetrieved)
{
	EU5::LocationManager theLocationManager;
	theLocationManager.registerLocation(1, "a");
	theLocationManager.registerLocation(2, "b");

	EXPECT_EQ(1, theLocationManager.getSeenLocationByID(1)->getID());
	EXPECT_EQ(2, theLocationManager.getSeenLocationByID(2)->getID());
}

TEST(EU5World_LocationManagerTests, LocationsCanBeParsed)
{
	EU5::LocationManager theLocationManager;
	theLocationManager.registerLocation(1, "a");
	theLocationManager.registerLocation(2, "b");

	std::stringstream input;
	input << "locations={\n";
	input << "1 = { owner = 4 }\n";
	input << "2 = { controller = 5 }\n";
	input << "}\n";
	theLocationManager.loadLocations(input);

	EXPECT_EQ(4, theLocationManager.getAllLocations().at("a")->getOwnerID());
	EXPECT_EQ(5, theLocationManager.getAllLocations().at("b")->getControllerID());
}

TEST(EU5World_LocationManagerTests, UnregisteredLocationsCannotBeParsed)
{
	EU5::LocationManager theLocationManager;
	std::stringstream input;
	input << "locations={\n";
	input << "1 = { owner = 4 }\n";
	input << "2 = { controller = 5 }\n";
	input << "}\n";
	theLocationManager.loadLocations(input);

	EXPECT_TRUE(theLocationManager.getAllLocations().empty());
}
