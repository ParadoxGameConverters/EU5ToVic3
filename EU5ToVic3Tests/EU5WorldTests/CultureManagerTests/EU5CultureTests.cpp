#include "CultureManager/EU5Culture.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>

TEST(EU5_EU5CultureTests, PrimitivesDefaultToDefault)
{
	std::stringstream input;
	const EU5::Culture culture(0, input);

	EXPECT_EQ(0, culture.getID());
	EXPECT_TRUE(culture.getName().empty());
	EXPECT_TRUE(culture.getCultureDefinition().empty());
	EXPECT_EQ(commonItems::Color(), culture.getColor());
	EXPECT_TRUE(culture.getLanguage().empty());
	EXPECT_EQ(0, culture.getSize());
	EXPECT_EQ(0, culture.getCulturalInfluence());
	EXPECT_EQ(0, culture.getCulturalTradition());
	EXPECT_TRUE(culture.getActive());
}

TEST(EU5_EU5CultureTests, DakelhCultureMinimalFieldsAreLoaded)
{
	std::stringstream input;
	input << "name=\"dakelh_culture\"\n";
	input << "size=3.392\n";
	input << "culture_definition=dakelh_culture\n";
	input << "color=rgb {68 252 27 }\n";
	input << "language=nadene_language\n";
	const EU5::Culture culture(0, input);

	EXPECT_EQ(0, culture.getID());
	EXPECT_EQ("dakelh_culture", culture.getName());
	EXPECT_EQ("dakelh_culture", culture.getCultureDefinition());
	EXPECT_EQ(commonItems::Color(std::array<int, 3>{68, 252, 27}), culture.getColor());
	EXPECT_EQ("nadene_language", culture.getLanguage());
	EXPECT_DOUBLE_EQ(3.392, culture.getSize());
	EXPECT_TRUE(culture.getActive());
}

TEST(EU5_EU5CultureTests, IrishCultureOptionalFieldsAreLoaded)
{
	std::stringstream input;
	input << "name=\"irish\"\n";
	input << "size=588.96385\n";
	input << "cultural_influence=264.00453\n";
	input << "cultural_tradition=809.56107\n";
	input << "culture_definition=irish\n";
	input << "color=rgb {46 140 36 }\n";
	input << "language=irish_dialect\n";
	const EU5::Culture culture(466, input);

	EXPECT_EQ(466, culture.getID());
	EXPECT_EQ("irish", culture.getName());
	EXPECT_DOUBLE_EQ(588.96385, culture.getSize());
	EXPECT_DOUBLE_EQ(264.00453, culture.getCulturalInfluence());
	EXPECT_DOUBLE_EQ(809.56107, culture.getCulturalTradition());
	EXPECT_EQ(commonItems::Color(std::array<int, 3>{46, 140, 36}), culture.getColor());
	EXPECT_EQ("irish_dialect", culture.getLanguage());
}

TEST(EU5_EU5CultureTests, RomanCultureIsInactive)
{
	std::stringstream input;
	input << "name=\"roman_culture\"\n";
	input << "culture_definition=roman_culture\n";
	input << "color=rgb {127 51 63 }\n";
	input << "active=no\n";
	input << "language=roman_dialect\n";
	const EU5::Culture culture(1412, input);

	EXPECT_EQ("roman_culture", culture.getName());
	EXPECT_FALSE(culture.getActive());
}
