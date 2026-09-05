#include "PopulationManager/PopulationManager.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>

TEST(EU5World_PopulationManagerTests, defaultPopulationManagerIsEmpty)
{
	const EU5::PopulationManager theManager;

	EXPECT_TRUE(theManager.getPops().empty());
}

TEST(EU5World_PopulationManagerTests, PopsCanBeLoadedFromDatabase)
{
	std::stringstream input;
	input << "database = {\n";
	input << "  1 = { type = nobles culture = 5 }\n";
	input << "  2 = { type = peasants culture = 6 }\n";
	input << "}\n";
	EU5::PopulationManager theManager;
	theManager.loadPopulation(input);

	EXPECT_EQ(2, theManager.getPops().size());
	EXPECT_EQ("nobles", theManager.getPopByID(1)->getType());
	EXPECT_EQ("peasants", theManager.getPopByID(2)->getType());
}

TEST(EU5World_PopulationManagerTests, accessingNonExistentPopReturnsNullptr)
{
	std::stringstream input;
	input << "database = {\n";
	input << "  1 = { type = nobles }\n";
	input << "}\n";
	EU5::PopulationManager theManager;
	theManager.loadPopulation(input);

	EXPECT_EQ(nullptr, theManager.getPopByID(42));
}
