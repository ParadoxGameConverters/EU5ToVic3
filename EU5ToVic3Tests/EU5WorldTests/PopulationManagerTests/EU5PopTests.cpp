#include "PopulationManager/EU5Pop.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::Pair;
using testing::UnorderedElementsAre;

TEST(EU5World_PopTests, PrimitivesDefaultToBlank)
{
	const EU5::Pop thePop;

	EXPECT_EQ(0, thePop.getID());
	EXPECT_TRUE(thePop.getType().empty());
	EXPECT_TRUE(thePop.getEstate().empty());
	EXPECT_EQ(0, thePop.getCultureID());
	EXPECT_TRUE(thePop.getStatus().empty());
	EXPECT_EQ(0, thePop.getReligionID());
	EXPECT_EQ(0, thePop.getSatisfaction());
	EXPECT_EQ(0, thePop.getSize());
	EXPECT_EQ(0, thePop.getLiteracy());
}

TEST(EU5World_PopTests, primitivesCanBeSet)
{
	std::stringstream input;
	input << "type = clergy\n";
	input << "estate = clergy_estate\n";
	input << "culture = 5\n";
	input << "status = Accepted\n";
	input << "religion = 4\n";
	input << "satisfaction = 55.5\n";
	input << "size = 0.06413\n";
	input << "literacy = 32.5\n";
	const EU5::Pop thePop(99, input);

	EXPECT_EQ(99, thePop.getID());
	EXPECT_EQ("clergy", thePop.getType());
	EXPECT_EQ("clergy_estate", thePop.getEstate());
	EXPECT_EQ(5, thePop.getCultureID());
	EXPECT_EQ("Accepted", thePop.getStatus());
	EXPECT_EQ(4, thePop.getReligionID());
	EXPECT_DOUBLE_EQ(55.5, thePop.getSatisfaction());
	EXPECT_DOUBLE_EQ(0.06413, thePop.getSize());
	EXPECT_DOUBLE_EQ(32.5, thePop.getLiteracy());
}
