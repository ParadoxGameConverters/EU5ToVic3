#include "ProvinceManager/EU5Location.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::UnorderedElementsAre;

TEST(EU5World_LocationTests, PrimitivesDefaultToBlank)
{
	const EU5::Location theLocation;

	EXPECT_EQ(0, theLocation.getID());
	EXPECT_TRUE(theLocation.getName().empty());
}
TEST(EU5World_LocationTests, primitivesCanBeSet)
{
	const EU5::Location theLocation(99, "theName");

	EXPECT_EQ(99, theLocation.getID());
	EXPECT_EQ("theName", theLocation.getName());
}
