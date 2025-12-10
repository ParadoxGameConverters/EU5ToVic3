#include "ProvinceManager/EU5Location.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::UnorderedElementsAre;

TEST(EU5_LocationTests, PrimitivesDefaultToBlank)
{
	const EU5::Location theLocation;

	EXPECT_EQ(0, theLocation.getID());
	EXPECT_TRUE(theLocation.getName().empty());
	EXPECT_EQ(0, theLocation.getOwnerID());
	EXPECT_EQ(0, theLocation.getControllerID());
	EXPECT_EQ(0, theLocation.getDevelopment());
	EXPECT_TRUE(theLocation.getLanguage().empty());
	EXPECT_TRUE(theLocation.getDialect().empty());
	EXPECT_TRUE(theLocation.getRank().empty());
	EXPECT_TRUE(theLocation.getCoreIDs().empty());
}

TEST(EU5_LocationTests, primitivesCanBeSet)
{
	EU5::Location theLocation(99, "theName");

	EXPECT_EQ(99, theLocation.getID());
	EXPECT_EQ("theName", theLocation.getName());

	std::stringstream input;
	input << "owner = 1\n";
	input << "controller = 2\n";
	input << "development = 3.5\n";
	input << "language = theLanguage\n";
	input << "dialect = theDialect\n";
	input << "rank = city\n";
	input << "cores = { 8 9 0 }\n";
	theLocation.parseData(input);

	EXPECT_EQ(1, theLocation.getOwnerID());
	EXPECT_EQ(2, theLocation.getControllerID());
	EXPECT_DOUBLE_EQ(3.5, theLocation.getDevelopment());
	EXPECT_EQ("theLanguage", theLocation.getLanguage());
	EXPECT_EQ("theDialect", theLocation.getDialect());
	EXPECT_EQ("city", theLocation.getRank());
	EXPECT_THAT(theLocation.getCoreIDs(), testing::UnorderedElementsAre(8, 9, 0));
}
