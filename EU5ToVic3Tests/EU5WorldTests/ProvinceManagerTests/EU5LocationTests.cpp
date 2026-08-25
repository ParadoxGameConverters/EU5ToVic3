#include "ProvinceManager/EU5Location.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::Pair;
using testing::UnorderedElementsAre;

TEST(EU5World_LocationTests, PrimitivesDefaultToBlank)
{
	const EU5::Location theLocation;

	EXPECT_EQ(0, theLocation.getID());
	EXPECT_TRUE(theLocation.getName().empty());
	EXPECT_EQ(0, theLocation.getOwnerID());
	EXPECT_EQ(0, theLocation.getControllerID());
	EXPECT_EQ(0, theLocation.getPreviousOwnerID());
	EXPECT_EQ(0, theLocation.getPreviousControllerID());
	EXPECT_EQ(0, theLocation.getDevelopment());
	EXPECT_EQ(0, theLocation.getProsperity());
	EXPECT_TRUE(theLocation.getLanguage().empty());
	EXPECT_TRUE(theLocation.getDialect().empty());
	EXPECT_TRUE(theLocation.getRank().empty());
	EXPECT_TRUE(theLocation.getOriginalRank().empty());
	EXPECT_TRUE(theLocation.getRawMaterial().empty());
	EXPECT_TRUE(theLocation.getCountriesWithCore().empty());

	EXPECT_EQ(0, theLocation.getReligionID());
	EXPECT_EQ(0, theLocation.getCultureID());
	EXPECT_EQ(0, theLocation.getSecondaryCultureID());
	EXPECT_EQ(0, theLocation.getCulturalUnity());

	EXPECT_EQ(date(), theLocation.getLastOwnerChange());
	EXPECT_EQ(date(), theLocation.getLastControllerChange());

	EXPECT_EQ(0, theLocation.getMaxRawMaterialWorkers());
	EXPECT_EQ(0, theLocation.getControl());
	EXPECT_FALSE(theLocation.getCapControl());
	EXPECT_FALSE(theLocation.getRoadToCapital());
	EXPECT_EQ(0, theLocation.getProximity());
	EXPECT_EQ(0, theLocation.getLocalProximityPropagation());
	EXPECT_EQ(0, theLocation.getValueFlow());

	EXPECT_EQ(0, theLocation.getProvinceID());
	EXPECT_EQ(0, theLocation.getTax());
	EXPECT_EQ(0, theLocation.getPossibleTax());
	EXPECT_TRUE(theLocation.getEstateTax().empty());
	EXPECT_TRUE(theLocation.getInstitutions().empty());

	EXPECT_TRUE(theLocation.getUnitIDs().empty());
	EXPECT_TRUE(theLocation.getPortIDs().empty());
	EXPECT_TRUE(theLocation.getPopIDs().empty());

	EXPECT_EQ(0, theLocation.getGarrison());
	EXPECT_TRUE(theLocation.getCounters().empty());

	EXPECT_EQ(0, theLocation.getMarketID());
	EXPECT_EQ(0, theLocation.getSecondBestMarketID());
	EXPECT_EQ(0, theLocation.getMarketParentID());
	EXPECT_EQ(0, theLocation.getMarketAccess());
	EXPECT_EQ(0, theLocation.getMarketAttraction());
	EXPECT_EQ(0, theLocation.getSecondBestMarketAccess());

	EXPECT_TRUE(theLocation.getIntegrationData().empty());
}

TEST(EU5World_LocationTests, primitivesCanBeSet)
{
	EU5::Location theLocation(99, "theName");

	EXPECT_EQ(99, theLocation.getID());
	EXPECT_EQ("theName", theLocation.getName());

	// TODO probably want to use real data yoinked from save file here
	std::stringstream input;
	input << "owner = 1\n";
	input << "controller = 2\n";
	input << "previous_owner = 3\n";
	input << "development = 3.5\n";
	input << "language = theLanguage\n";
	input << "dialect = theDialect\n";
	input << "rank = city\n";
	input << "original_rank = town\n";
	input << "raw_material = clay\n";
	input << "cores = { 8 9 0 }\n";
	input << "religion = 4\n";
	input << "culture = 5\n";
	input << "secondary_culture = 6\n";
	input << "cultural_unity = 6.5\n";
	input << "last_owner_change = 1337.4.1\n";
	input << "last_controller_change = 1337.4.1\n";
	input << "max_raw_material_workers = 7\n";
	input << "control = 8\n";
	input << "cap_control = yes\n";
	input << "road_to_capital = yes\n";
	input << "proximity = 100\n";
	input << "local_proximity_propagation = 9.5\n";
	input << "value_flow = 10.5\n";
	input << "province = 11\n";
	input << "tax = 12.5\n";
	input << "possible_tax = 13.5\n";
	input << "estate_tax = { nobles_estate = 0.5 clergy_estate = 0.5 }\n";
	input << "estate_possible_tax = { nobles_estate = 1.5 clergy_estate = 1.5 }\n";
	input << "institutions = { feudalism = 100 legalism = 100 }\n";
	input << "units = { 14 }\n";
	input << "port = { 14 }\n";
	input << "population = { pop_stats = { nobles = { population_ratio = 0.15 } } pops = { 0 1 2 3 4 51127 51128 5 6 } }\n";
	input << "garrison = 0.5\n";
	input << "counters = { Pops = 16 TradePath = 17 }\n";
	input << "market = 18\n";
	input << "second_best_market = 19\n";
	input << "market_parent = 20\n";
	input << "market_access = 1\n";
	input << "market_attraction = 21.5\n";
	input << "second_best_market_access = 0.5\n";
	input << "previous_controller = 22\n";
	input << "prosperity = 22.5\n";
	input << "integration_data = { { integration = integrated integration_progress = 23.5 integration_owner = 24 } }\n";
	theLocation.parseData(input);

	EXPECT_EQ(1, theLocation.getOwnerID());
	EXPECT_EQ(2, theLocation.getControllerID());
	EXPECT_EQ(3, theLocation.getPreviousOwnerID());
	EXPECT_EQ(22, theLocation.getPreviousControllerID());
	EXPECT_DOUBLE_EQ(3.5, theLocation.getDevelopment());
	EXPECT_DOUBLE_EQ(22.5, theLocation.getProsperity());
	EXPECT_EQ("theLanguage", theLocation.getLanguage());
	EXPECT_EQ("theDialect", theLocation.getDialect());
	EXPECT_EQ("city", theLocation.getRank());
	EXPECT_EQ("town", theLocation.getOriginalRank());
	EXPECT_EQ("clay", theLocation.getRawMaterial());
	EXPECT_THAT(theLocation.getCountriesWithCore(), UnorderedElementsAre(8, 9, 0));

	EXPECT_EQ(4, theLocation.getReligionID());
	EXPECT_EQ(5, theLocation.getCultureID());
	EXPECT_EQ(6, theLocation.getSecondaryCultureID());
	EXPECT_DOUBLE_EQ(6.5, theLocation.getCulturalUnity());

	EXPECT_EQ(date(1337, 4, 1), theLocation.getLastOwnerChange());
	EXPECT_EQ(date(1337, 4, 1), theLocation.getLastControllerChange());

	EXPECT_EQ(7, theLocation.getMaxRawMaterialWorkers());
	EXPECT_DOUBLE_EQ(8, theLocation.getControl());
	EXPECT_TRUE(theLocation.getCapControl());
	EXPECT_TRUE(theLocation.getRoadToCapital());
	EXPECT_DOUBLE_EQ(100, theLocation.getProximity());
	EXPECT_DOUBLE_EQ(9.5, theLocation.getLocalProximityPropagation());
	EXPECT_DOUBLE_EQ(10.5, theLocation.getValueFlow());

	EXPECT_EQ(11, theLocation.getProvinceID());
	EXPECT_DOUBLE_EQ(12.5, theLocation.getTax());
	EXPECT_DOUBLE_EQ(13.5, theLocation.getPossibleTax());
	EXPECT_THAT(theLocation.getEstateTax(),
		 UnorderedElementsAre(Pair("nobles_estate", EU5::EstateTax{0.5, 1.5}), Pair("clergy_estate", EU5::EstateTax{0.5, 1.5})));
	EXPECT_THAT(theLocation.getInstitutions(), UnorderedElementsAre(Pair("feudalism", 100), Pair("legalism", 100)));

	EXPECT_THAT(theLocation.getUnitIDs(), UnorderedElementsAre(14));
	EXPECT_THAT(theLocation.getPortIDs(), UnorderedElementsAre(14));
	EXPECT_THAT(theLocation.getPopIDs(), UnorderedElementsAre(0, 1, 2, 3, 4, 51127, 51128, 5, 6));

	EXPECT_DOUBLE_EQ(0.5, theLocation.getGarrison());
	EXPECT_THAT(theLocation.getCounters(), UnorderedElementsAre(Pair("Pops", 16), Pair("TradePath", 17)));

	EXPECT_EQ(18, theLocation.getMarketID());
	EXPECT_EQ(19, theLocation.getSecondBestMarketID());
	EXPECT_EQ(20, theLocation.getMarketParentID());
	EXPECT_DOUBLE_EQ(1, theLocation.getMarketAccess());
	EXPECT_DOUBLE_EQ(21.5, theLocation.getMarketAttraction());
	EXPECT_DOUBLE_EQ(0.5, theLocation.getSecondBestMarketAccess());

	EXPECT_THAT(theLocation.getIntegrationData(), UnorderedElementsAre(Pair(24, EU5::Integration{"integrated", 23.5})));
}
