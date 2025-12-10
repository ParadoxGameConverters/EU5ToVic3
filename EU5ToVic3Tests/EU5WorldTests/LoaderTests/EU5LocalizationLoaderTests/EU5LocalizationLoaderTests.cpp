#include "CommonFunctions.h"
#include "EU5LocalizationLoader/EU5LocalizationLoader.h"
#include "gtest/gtest.h"

TEST(EU5_LocalizationLoaderTests, localisationsCanBeLoadedAndMatched)
{
	std::stringstream input;
	input << commonItems::utf8BOM << "l_english:\n";
	input << " key1: \"value 1\" # comment\n";
	input << " key2: \"value \"subquoted\" 2\"\n";
	std::stringstream input2;
	input2 << commonItems::utf8BOM << "l_french:\n";
	input2 << " key1: \"valuee 1\"\n";
	input2 << " key2: \"valuee \"subquoted\" 2\"\n";
	std::stringstream input3;
	input3 << commonItems::utf8BOM << "l_english:\n";
	input3 << " key1: \"replaced value 1\"\n";

	EU5::EU5LocalizationLoader locs;
	locs.scrapeStream(input, "english");
	locs.scrapeStream(input2, "french");
	locs.scrapeStream(input3, "english");

	EXPECT_EQ("replaced value 1", locs.getLocBlockForKey("key1")->english);
	EXPECT_EQ("value \"subquoted\" 2", locs.getLocBlockForKey("key2")->english);
	EXPECT_EQ("valuee 1", locs.getLocBlockForKey("key1")->french);
	EXPECT_EQ("valuee \"subquoted\" 2", locs.getLocBlockForKey("key2")->french);
}

TEST(EU5_LocalizationLoaderTests, localisationsReturnNulloptForMissingKey)
{
	const EU5::EU5LocalizationLoader locs;

	EXPECT_FALSE(locs.getLocBlockForKey("key1"));
}

TEST(EU5_LocalizationLoaderTests, localisationsReturnsEnglishForMissingLanguage)
{
	EU5::EU5LocalizationLoader locs;
	std::stringstream input;
	input << commonItems::utf8BOM << "l_english:\n";
	input << " key1: \"value 1\" # comment\n";
	locs.scrapeStream(input, "english");

	EXPECT_EQ("value 1", locs.getLocBlockForKey("key1")->french);
}

TEST(EU5_LocalizationLoaderTests, nestedLocalisationsCanBeUnravelled)
{
	EU5::EU5LocalizationLoader locs;
	std::stringstream input;
	input << commonItems::utf8BOM << "l_english:\n";
	input << " nested_key: \"This $n$ has nested $locs$\"\n";
	input << " n: \"$noun$\"\n";
	input << " locs: \"localizations\"\n";
	input << " noun: \"string\"\n";

	locs.scrapeStream(input, "english");
	auto copyblock = locs.getLocBlockForKey("nested_key");
	locs.unravelNestedLocs(copyblock.value());
	EXPECT_EQ("This string has nested localizations", copyblock->english);
}

TEST(EU5_LocalizationLoaderTests, stringsCanBeSelectedFor)
{
	std::string a = "I_do_not_fear_the_big_blue_blob.";
	std::string b = "Ich f�rchte den gro�en  blauen Klecks nicht.";
	std::string c = "Je$n'ai$pas$peur$du$gros$blob$bleu.";
	std::string d = "No__le__temo__a__la__gran__mancha__azul.";

	EXPECT_EQ(EU5::getLeadStr(a), "I");
	EXPECT_EQ(EU5::getLeadStr(d), "No");
	EXPECT_EQ(EU5::getTailStr(a), "do_not_fear_the_big_blue_blob.");
	EXPECT_EQ(EU5::getTailStr(d), "_le__temo__a__la__gran__mancha__azul.");

	EXPECT_EQ(EU5::getLeadStr(d, 1, "__"), "No");
	EXPECT_EQ(EU5::getTailStr(d, 1, "__"), "le__temo__a__la__gran__mancha__azul.");

	EXPECT_EQ(EU5::getLeadStr(b, 1, " "), "Ich");
	EXPECT_EQ(EU5::getLeadStr(b, 1, "  "), "Ich f�rchte den gro�en");
	EXPECT_EQ(EU5::getTailStr(b, 1, " "), "f�rchte den gro�en  blauen Klecks nicht.");
	EXPECT_EQ(EU5::getTailStr(b, 1, "  "), "blauen Klecks nicht.");
	EXPECT_EQ(EU5::getTailStr(b, 4, " "), " blauen Klecks nicht.");

	EXPECT_EQ(EU5::getLeadStr(c, 5, "$"), "Je$n'ai$pas$peur$du");
	EXPECT_EQ(EU5::getTailStr(c, 5, "$"), "gros$blob$bleu.");
	EXPECT_EQ(EU5::getLeadStr(d, 6), "No__le__temo_");
	EXPECT_EQ(EU5::getLeadStr(d, 6, "__"), "No__le__temo__a__la__gran");
	EXPECT_EQ(EU5::getTailStr(d, 6), "a__la__gran__mancha__azul.");
	EXPECT_EQ(EU5::getTailStr(d, 6, "__"), "mancha__azul.");
}

TEST(EU5_LocalizationLoaderTests, trivialEmphasisCanBeRemoved)
{
	EU5::EU5LocalizationLoader locs;
	std::stringstream input;
	input << commonItems::utf8BOM << "l_english:\n";
	input << " key: \"random #F Halale, or truth, of halale. and recognition.#!\"\n";

	locs.scrapeStream(input, "english");
	auto copyBlock = locs.getLocBlockForKey("key");
	locs.removeEmphasis(*copyBlock);
	EXPECT_EQ("random Halale, or truth, of halale. and recognition.", copyBlock->english);
}

TEST(EU5_LocalizationLoaderTests, multipleTrivialEmphasisCanBeRemoved)
{
	EU5::EU5LocalizationLoader locs;
	std::stringstream input;
	input << commonItems::utf8BOM << "l_english:\n";
	input << " key: \"random #F Halale, or truth#!, of #EMPHASIS halale. and recognition.#!\"\n";

	locs.scrapeStream(input, "english");
	auto copyBlock = locs.getLocBlockForKey("key");
	locs.removeEmphasis(*copyBlock);
	EXPECT_EQ("random Halale, or truth, of halale. and recognition.", copyBlock->english);
}

TEST(EU5_LocalizationLoaderTests, nestedEmphasisCanBeRemoved)
{
	EU5::EU5LocalizationLoader locs;
	std::stringstream input;
	input << commonItems::utf8BOM << "l_english:\n";
	input << " key: \"#RANDOMTESTMARK #TESTMARK Halale#!, or truth, of #TESTMARK2 halale#!. and recognition.#!\"\n";

	locs.scrapeStream(input, "english");
	auto copyBlock = locs.getLocBlockForKey("key");
	locs.removeEmphasis(*copyBlock);
	EXPECT_EQ("Halale, or truth, of halale. and recognition.", copyBlock->english);
}

TEST(EU5_LocalizationLoaderTests, complexEmphasisCanBeRemoved)
{
	EU5::EU5LocalizationLoader locs;
	std::stringstream input;
	input << commonItems::utf8BOM << "l_english:\n";
	input << " key: \"#F #italic Halale#!, or truth, of #italic halale#!. and recognition.#!\"\n";

	locs.scrapeStream(input, "english");
	auto copyBlock = locs.getLocBlockForKey("key");
	locs.removeEmphasis(*copyBlock);
	EXPECT_EQ("Halale, or truth, of halale. and recognition.", copyBlock->english);
}

TEST(EU5_LocalizationLoaderTests, malformedEmphasisCanBeRemoved)
{
	EU5::EU5LocalizationLoader locs;
	std::stringstream input;
	input << commonItems::utf8BOM << "l_english:\n";
	input << " key: \"random Halale#!, or# truth, of # halale#!. and recognition.#\"\n";

	locs.scrapeStream(input, "english");
	auto copyBlock = locs.getLocBlockForKey("key");
	locs.removeEmphasis(*copyBlock);
	EXPECT_EQ("random Halale, or truth, of  halale. and recognition.", copyBlock->english);
}
