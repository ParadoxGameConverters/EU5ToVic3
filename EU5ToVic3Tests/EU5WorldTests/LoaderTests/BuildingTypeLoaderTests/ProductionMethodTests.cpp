#include "BuildingTypeLoader/ProductionMethod.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::UnorderedElementsAre;

TEST(EU5_ProductionMethodTests, PrimitivesDefaultToDefault)
{
	std::stringstream input;
	const EU5::ProductionMethod method(input);

	EXPECT_TRUE(method.getProducedGood().empty());
	EXPECT_TRUE(method.getInputGoods().empty());
}

// real building_types/production_beer.txt: brewery.unique_production_methods.honey_brewery_maintenance
TEST(EU5_ProductionMethodTests, HoneyBreweryMaintenanceIsLoaded)
{
	std::stringstream input;
	input << "beeswax = 0.2603\n";
	input << "millet = 0.5206\n";
	input << "lumber = 0.208\n";
	input << "tools = 0.1045\n";
	input << "produced = beer\n";
	input << "output = 1\n";
	input << "debug_max_profit = guild_profit_margin\n";
	input << "category = guild_input\n";
	const EU5::ProductionMethod method(input);

	EXPECT_EQ("beer", method.getProducedGood());
	EXPECT_THAT(method.getInputGoods(), UnorderedElementsAre("beeswax", "millet", "lumber", "tools"));
}

// real building_types/production_beer.txt: brewery_mill.unique_production_methods.brewery_mill_maintenance
TEST(EU5_ProductionMethodTests, BreweryMillMaintenanceIsLoaded)
{
	std::stringstream input;
	input << "lumber = 0.5038\n";
	input << "wheat = 4.4144\n";
	input << "tools = 0.2519\n";
	input << "produced = beer\n";
	input << "output = 4\n";
	input << "debug_max_profit = mills_profit_margin\n";
	input << "category = mills_input\n";
	const EU5::ProductionMethod method(input);

	EXPECT_EQ("beer", method.getProducedGood());
	EXPECT_THAT(method.getInputGoods(), UnorderedElementsAre("lumber", "wheat", "tools"));
}
