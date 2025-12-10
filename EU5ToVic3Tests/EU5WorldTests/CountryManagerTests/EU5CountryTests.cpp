#include "CountryManager/EU5Country.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::ElementsAre;
using testing::UnorderedElementsAre;

TEST(EU5_EU5CountryTests, PrimitivesDefaultToDefault)
{
	std::stringstream input;
	const EU5::Country country(0, input);

	EXPECT_EQ(0, country.getID());
	EXPECT_TRUE(country.getTag().empty());
	EXPECT_TRUE(country.getCountryName().empty());
	EXPECT_TRUE(country.getAdjective().empty());
	EXPECT_TRUE(country.getFlag().empty());
	EXPECT_TRUE(country.getType().empty());
	EXPECT_TRUE(country.getHistorical().empty());
	EXPECT_TRUE(country.getCountryType().empty());
	EXPECT_FALSE(country.isRevolutionaryTarget());
}

TEST(EU5_EU5CountryTests, PrimitivesCanBeLoaded)
{
	std::stringstream input;
	input << "country_name=\"AAA\"\n";
	input << "flag=\"NOR\"\n";
	input << "type=\"location\"\n";
	input << "historical=\"SWE\"\n";
	input << "country_type=\"Real\"\n";
	EU5::Country country(42, input);
	country.setTag("SWE");
	country.setRevolutionaryTarget();

	EXPECT_EQ(42, country.getID());
	EXPECT_EQ("SWE", country.getTag());
	EXPECT_EQ("AAA", country.getCountryName());
	EXPECT_TRUE(country.getAdjective().empty());
	EXPECT_EQ("NOR", country.getFlag());
	EXPECT_EQ("location", country.getType());
	EXPECT_EQ("SWE", country.getHistorical());
	EXPECT_EQ("Real", country.getCountryType());
	EXPECT_TRUE(country.isRevolutionaryTarget());
}

TEST(EU5_EU5CountryTests, ExpandedNameCanBeLoaded)
{
	std::stringstream input;
	input << "country_name={\n";
	input << "name=\"NOR\"\n";
	input << "adjective=\"DAN\"\n";
	input << "}\n";
	EU5::Country country(42, input);

	EXPECT_EQ("NOR", country.getCountryName());
	EXPECT_EQ("DAN", country.getAdjective());
}
