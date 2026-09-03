#include "CommonFunctions.h"
#include "CountryManager/EU5CountryManager.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::UnorderedElementsAre;

TEST(EU5_CountryManagerTests, primitivesDefaultToBlank)
{
	const EU5::CountryManager manager;

	EXPECT_TRUE(manager.getCountries().empty());
}

TEST(EU5_CountryManagerTests, countriesCanBeRegistered)
{
	std::stringstream input;
	input << "tags = {\n";
	input << "  1=SWE\n";
	input << "	2=DAN\n";
	input << "	3=NOR\n";
	input << "}\n";
	EU5::CountryManager manager;
	manager.loadCountries(input);

	EXPECT_EQ(3, manager.getTagRegistry().size());
	EXPECT_EQ("SWE", manager.getTagRegistry().at(1));
	EXPECT_EQ("DAN", manager.getTagRegistry().at(2));
	EXPECT_EQ("NOR", manager.getTagRegistry().at(3));
}

TEST(EU5_CountryManagerTests, SpecialCountriesAreIgnored)
{
	std::stringstream input;
	input << "tags = {\n";
	input << "	1=\"---\"\n";
	input << "	2=PIR\n";
	input << "	3=MER\n";
	input << "	4=DUMMY\n";
	input << "}\n";
	EU5::CountryManager manager;
	manager.loadCountries(input);

	EXPECT_TRUE(manager.getTagRegistry().empty());
}

TEST(EU5_CountryManagerTests, UnregisteredCountriesCannotBeLoaded)
{
	std::stringstream input;
	input << "tags = {\n";
	input << "  1 = AAA\n";
	input << "}\n";
	input << "database = {\n";
	input << "  3 = { country_name = BBB } \n";
	input << "}\n";
	EU5::CountryManager manager;
	manager.loadCountries(input);

	EXPECT_TRUE(manager.getCountries().empty());
}

TEST(EU5_CountryManagerTests, RegisteredCountriesCanBeLoaded)
{
	std::stringstream input;
	input << "tags = {\n";
	input << "  1 = AAA\n";
	input << "  2 = BBB\n";
	input << "}\n";
	input << "database = {\n";
	input << "  1 = { country_name = AAA } \n";
	input << "  2 = { country_name = BBB } \n";
	input << "}\n";
	EU5::CountryManager manager;
	manager.loadCountries(input);

	EXPECT_EQ(2, manager.getTagRegistry().size());
	EXPECT_EQ(2, manager.getCountries().size());
	EXPECT_EQ("AAA", manager.getCountryByID(1)->getCountryName());
	EXPECT_EQ("BBB", manager.getCountryByID(2)->getCountryName());
}

TEST(EU5_CountryManagerTests, RevolutionaryTargetCanBeSet)
{
	std::stringstream input;
	input << "tags = {\n";
	input << "  1 = AAA\n";
	input << "  2 = BBB\n";
	input << "}\n";
	input << "revolutionary_target = 2\n";
	input << "database = {\n";
	input << "  1 = { country_name = AAA } \n";
	input << "  2 = { country_name = BBB } \n";
	input << "}\n";
	EU5::CountryManager manager;
	manager.loadCountries(input);

	EXPECT_EQ(2, manager.getCountries().size());
	EXPECT_FALSE(manager.getCountryByID(1)->isRevolutionaryTarget());
	EXPECT_TRUE(manager.getCountryByID(2)->isRevolutionaryTarget());
}
