#include "BuildingManager/EU5Building.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>

TEST(EU5_EU5BuildingTests, PrimitivesDefaultToDefault)
{
	std::stringstream input;
	const EU5::Building building(0, input);

	EXPECT_EQ(0, building.getID());
	EXPECT_TRUE(building.getType().empty());
	EXPECT_EQ(0, building.getLevel());
	EXPECT_EQ(0, building.getLocationID());
	EXPECT_EQ(0, building.getOwnerEstateID());
	EXPECT_EQ(0, building.getPopID());
	EXPECT_EQ(0, building.getEmployed());
	EXPECT_EQ(0, building.getEmploymentRequirement());
	EXPECT_TRUE(building.getEmploymentRequirementStatus().empty());
	EXPECT_EQ(0, building.getEstablishmentProgress());
	EXPECT_EQ(0, building.getLastMonthsProfit());
	EXPECT_EQ(0, building.getUpkeep());
	EXPECT_FALSE(building.getOpen());
	EXPECT_FALSE(building.getSubsidized());
	EXPECT_TRUE(building.getProductionMethod().empty());
	EXPECT_EQ(0, building.getInput());
}

// real save data, building_manager.database.15067 (Bohemia save, 1339.4.1)
TEST(EU5_EU5BuildingTests, HanseaticKontorFieldsAreLoaded)
{
	std::stringstream input;
	input << "type=hanseatic_kontor\n";
	input << "level=3\n";
	input << "employed=0.6\n";
	input << "upkeep=0.56469\n";
	input << "location=846\n";
	input << "owner=1772\n";
	input << "pop=50255\n";
	input << "establishment_progress=0\n";
	const EU5::Building building(15067, input);

	EXPECT_EQ(15067, building.getID());
	EXPECT_EQ("hanseatic_kontor", building.getType());
	EXPECT_EQ(3, building.getLevel());
	EXPECT_DOUBLE_EQ(0.6, building.getEmployed());
	EXPECT_DOUBLE_EQ(0.56469, building.getUpkeep());
	EXPECT_EQ(846, building.getLocationID());
	EXPECT_EQ(1772, building.getOwnerEstateID());
	EXPECT_EQ(50255, building.getPopID());
	EXPECT_EQ(0, building.getEstablishmentProgress());
}

// real save data, building_manager.database.2 (Bohemia save, 1339.4.1)
TEST(EU5_EU5BuildingTests, ToolsGuildEmploymentFieldsAreLoaded)
{
	std::stringstream input;
	input << "type=tools_guild\n";
	input << "level=1\n";
	input << "employment_requirement=70\n";
	input << "employment_requirement_status=LayingOff\n";
	input << "location=1\n";
	input << "open=no\n";
	input << "owner=3\n";
	input << "last_months_profit=-0.32048\n";
	input << "establishment_progress=240\n";
	const EU5::Building building(2, input);

	EXPECT_EQ("tools_guild", building.getType());
	EXPECT_EQ(70, building.getEmploymentRequirement());
	EXPECT_EQ("LayingOff", building.getEmploymentRequirementStatus());
	EXPECT_EQ(1, building.getLocationID());
	EXPECT_FALSE(building.getOpen());
	EXPECT_EQ(3, building.getOwnerEstateID());
	EXPECT_DOUBLE_EQ(-0.32048, building.getLastMonthsProfit());
	EXPECT_EQ(240, building.getEstablishmentProgress());
}

// real save data, building_manager.database.1263 (Bohemia save, 1339.4.1)
TEST(EU5_EU5BuildingTests, PotteryGuildSubsidizedFlagIsLoaded)
{
	std::stringstream input;
	input << "type=pottery_guild\n";
	input << "level=1\n";
	input << "employed=0.1\n";
	input << "location=3164\n";
	input << "subsidized=yes\n";
	input << "owner=1455\n";
	input << "last_months_profit=0.0552\n";
	input << "establishment_progress=240\n";
	const EU5::Building building(1263, input);

	EXPECT_EQ("pottery_guild", building.getType());
	EXPECT_EQ(3164, building.getLocationID());
	EXPECT_TRUE(building.getSubsidized());
	EXPECT_EQ(1455, building.getOwnerEstateID());
}

// real save data, building_manager.database.1 (Bohemia save, 1339.4.1)
TEST(EU5_EU5BuildingTests, ProductionMethodBlockIsParsed)
{
	std::stringstream input;
	input << "type=temple\n";
	input << "level=1\n";
	input << "employed=0.05\n";
	input << "upkeep=0.05255\n";
	input << "location=1\n";
	input << "owner=3\n";
	input << "establishment_progress=0\n";
	input << "temple_maintenance={\n";
	input << "  missing={\n";
	input << "    demand=temple_maintenance\n";
	input << "    glass=0.0128\n";
	input << "  }\n";
	input << "  input=0.83333\n";
	input << "}\n";
	const EU5::Building building(1, input);

	EXPECT_EQ("temple_maintenance", building.getProductionMethod());
	EXPECT_DOUBLE_EQ(0.83333, building.getInput());
}
