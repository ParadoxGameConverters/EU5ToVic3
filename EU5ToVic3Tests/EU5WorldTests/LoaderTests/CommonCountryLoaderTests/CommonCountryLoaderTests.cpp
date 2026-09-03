#include "CommonCountryLoader/CommonCountryLoader.h"
#include "gtest/gtest.h"

TEST(EU5_CommonCountryLoaderTests, CommonsCanBeLoadedFromCommonCountries)
{
	std::stringstream input;
	input << "TST = { \n";
	input << "color = hsv {0 0 0.5 }\n";
	input << "color2 = rgb { 241 198 188 }\n";
	input << "culture_definition = turkish_culture\n";
	input << "religion_definition = shia\n";
	input << "}\n";

	input << "TST2 = {\n";
	input << "color = hsv {0 0.2 0.2 }\n";
	input << "color2 = rgb { 166 128 194 }\n";
	input << "culture_definition = arabic_culture\n";
	input << "religion_definition = islamic\n";
	input << "}\n";

	EU5::CommonCountryLoader loader;
	loader.loadCommonCountries(input);

	EXPECT_EQ("= rgb { 127 127 127 }", loader.getColor1("TST")->outputRgb());
	EXPECT_EQ("= rgb { 241 198 188 }", loader.getColor2("TST")->outputRgb());
	EXPECT_EQ("turkish_culture", *loader.getCountryCulture("TST"));
	EXPECT_EQ("shia", *loader.getCountryReligion("TST"));

	EXPECT_EQ("= rgb { 51 40 40 }", loader.getColor1("TST2")->outputRgb());
	EXPECT_EQ("= rgb { 166 128 194 }", loader.getColor2("TST2")->outputRgb());
	EXPECT_EQ("arabic_culture", *loader.getCountryCulture("TST2"));
	EXPECT_EQ("islamic", *loader.getCountryReligion("TST2"));
}

TEST(EU5_CommonCountryLoaderTests, NulloptIsReturnedForMismatches)
{
	std::stringstream input;
	input << "TST = { \n";
	input << "color = hsv {0 0 15 }\n";
	input << "color2 = rgb { 241 198 188 }\n";
	input << "culture_definition = turkish_culture\n";
	input << "religion_definition = shia\n";
	input << "}\n";

	input << "TST2 = {\n";
	input << "color = hsv {0 15 0 }\n";
	input << "color2 = rgb { 166 128 194 }\n";
	input << "culture_definition = arabic_culture\n";
	input << "religion_definition = islamic\n";
	input << "}\n";

	EU5::CommonCountryLoader loader;
	loader.loadCommonCountries(input);

	EXPECT_EQ(std::nullopt, loader.getColor1("BLA"));
	EXPECT_EQ(std::nullopt, loader.getColor2("BLA"));
	EXPECT_EQ(std::nullopt, loader.getCountryCulture("BLA"));
	EXPECT_EQ(std::nullopt, loader.getCountryReligion("BLA"));
}
