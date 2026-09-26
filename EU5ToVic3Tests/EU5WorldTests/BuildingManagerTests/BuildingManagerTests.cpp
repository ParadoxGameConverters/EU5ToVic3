#include "BuildingManager/BuildingManager.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::UnorderedElementsAre;

TEST(EU5_BuildingManagerTests, primitivesDefaultToBlank)
{
	const EU5::BuildingManager manager;

	EXPECT_TRUE(manager.getBuildings().empty());
}

// real save data, building_manager.database.{0,1,2} (Bohemia save, 1339.4.1), all at location 1
TEST(EU5_BuildingManagerTests, BuildingsCanBeLoaded)
{
	std::stringstream input;
	input << "database = {\n";
	input << "  0 = { type=brewery location=1 owner=3 }\n";
	input << "  1 = { type=temple location=1 owner=3 }\n";
	input << "  2 = { type=tools_guild location=1 owner=3 }\n";
	input << "}\n";
	EU5::BuildingManager manager;
	manager.loadBuildings(input);

	EXPECT_EQ(3, manager.getBuildings().size());
	EXPECT_EQ("brewery", manager.getBuildingByID(0)->getType());
	EXPECT_EQ("temple", manager.getBuildingByID(1)->getType());
	EXPECT_EQ("tools_guild", manager.getBuildingByID(2)->getType());
}

TEST(EU5_BuildingManagerTests, MissingBuildingReturnsNullptr)
{
	const EU5::BuildingManager manager;

	EXPECT_EQ(nullptr, manager.getBuildingByID(99));
}

// real save data, building_manager.database.{5339,5340,5341} (Bohemia save, 1339.4.1) - 5340 is a bare "none" sentinel
TEST(EU5_BuildingManagerTests, NoneSentinelsAreSkippedWithoutLosingFollowingBuildings)
{
	std::stringstream input;
	input << "database = {\n";
	input << "  5339 = { type=paper_guild location=2683 owner=1251 }\n";
	input << "  5340 = none\n";
	input << "  5341 = { type=granary location=2683 owner=1251 }\n";
	input << "}\n";
	EU5::BuildingManager manager;
	manager.loadBuildings(input);

	EXPECT_EQ(2, manager.getBuildings().size());
	EXPECT_EQ("paper_guild", manager.getBuildingByID(5339)->getType());
	EXPECT_EQ(nullptr, manager.getBuildingByID(5340));
	EXPECT_EQ("granary", manager.getBuildingByID(5341)->getType());
}

// real save data: buildings 0,1,2 sit at location 1; building 15067 (hanseatic_kontor) sits at location 846
TEST(EU5_BuildingManagerTests, BuildingIDsCanBeFetchedByLocation)
{
	std::stringstream input;
	input << "database = {\n";
	input << "  0 = { type=brewery location=1 owner=3 }\n";
	input << "  1 = { type=temple location=1 owner=3 }\n";
	input << "  2 = { type=tools_guild location=1 owner=3 }\n";
	input << "  15067 = { type=hanseatic_kontor location=846 owner=1772 }\n";
	input << "}\n";
	EU5::BuildingManager manager;
	manager.loadBuildings(input);

	EXPECT_THAT(manager.getBuildingIDsAtLocation(1), UnorderedElementsAre(0, 1, 2));
	EXPECT_THAT(manager.getBuildingIDsAtLocation(846), UnorderedElementsAre(15067));
	EXPECT_TRUE(manager.getBuildingIDsAtLocation(99).empty());
}
