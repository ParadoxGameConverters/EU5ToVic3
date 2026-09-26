#include "BuildingTypeLoader/BuildingTypeLoader.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::UnorderedElementsAre;

// trimmed from real building_types/production_beer.txt: brewery and brewery_mill
TEST(EU5_BuildingTypeLoaderTests, PopTypesAreKeyedByBuildingType)
{
	std::stringstream input;
	input << "brewery = {\n";
	input << "  pop_type = burghers\n";
	input << "  category = consumer_goods_category\n";
	input << "}\n";
	input << "brewery_mill = {\n";
	input << "  pop_type = laborers\n";
	input << "  is_mill = yes\n";
	input << "}\n";
	EU5::BuildingTypeLoader loader;
	loader.loadBuildingTypes(input);

	EXPECT_EQ("burghers", *loader.getPopType("brewery"));
	EXPECT_EQ("laborers", *loader.getPopType("brewery_mill"));
}

TEST(EU5_BuildingTypeLoaderTests, NulloptIsReturnedForMismatches)
{
	std::stringstream input;
	input << "brewery = {\n";
	input << "  pop_type = burghers\n";
	input << "}\n";
	EU5::BuildingTypeLoader loader;
	loader.loadBuildingTypes(input);

	EXPECT_EQ(std::nullopt, loader.getPopType("nonexistent"));
	EXPECT_EQ(std::nullopt, loader.getProductionMethod("nonexistent"));
}

// trimmed from real building_types/production_beer.txt: brewery.unique_production_methods and brewery_mill.unique_production_methods
TEST(EU5_BuildingTypeLoaderTests, ProductionMethodsAreLoadedPerBuildingType)
{
	std::stringstream input;
	input << "brewery = {\n";
	input << "  pop_type = burghers\n";
	input << "  unique_production_methods = {\n";
	input << "    honey_brewery_maintenance = {\n";
	input << "      beeswax = 0.2603\n";
	input << "      millet = 0.5206\n";
	input << "      lumber = 0.208\n";
	input << "      tools = 0.1045\n";
	input << "      produced = beer\n";
	input << "      output = 1\n";
	input << "    }\n";
	input << "  }\n";
	input << "}\n";
	input << "brewery_mill = {\n";
	input << "  pop_type = laborers\n";
	input << "  unique_production_methods = {\n";
	input << "    brewery_mill_maintenance = {\n";
	input << "      lumber = 0.5038\n";
	input << "      wheat = 4.4144\n";
	input << "      tools = 0.2519\n";
	input << "      produced = beer\n";
	input << "      output = 4\n";
	input << "    }\n";
	input << "  }\n";
	input << "}\n";
	EU5::BuildingTypeLoader loader;
	loader.loadBuildingTypes(input);

	const auto& honeyBrewery = loader.getProductionMethod("honey_brewery_maintenance");
	EXPECT_EQ("beer", honeyBrewery->getProducedGood());
	EXPECT_THAT(honeyBrewery->getInputGoods(), UnorderedElementsAre("beeswax", "millet", "lumber", "tools"));

	const auto& breweryMill = loader.getProductionMethod("brewery_mill_maintenance");
	EXPECT_EQ("beer", breweryMill->getProducedGood());
	EXPECT_THAT(breweryMill->getInputGoods(), UnorderedElementsAre("lumber", "wheat", "tools"));
}
