#include "CommonCultureLoader/CommonCultureLoader.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::ElementsAre;

// make sure y'all keep using save data for tests - better imho
TEST(EU5_CommonCultureLoaderTests, CommonsCanBeLoadedFromCommonCultures)
{
	std::stringstream input;
	input << "aukstaitian = {\n";
	input << "  language = lithuanian_dialect\n";
	input << "  color = map_LIT\n";
	input << "  culture_groups = {\n";
	input << "    lithuanian_group\n";
	input << "    baltic_group\n";
	input << "  }\n";
	input << "}\n";
	input << "curonian = {\n";
	input << "  language = western_baltic_dialect\n";
	input << "  color = map_curonian\n";
	input << "  culture_groups = {\n";
	input << "    baltic_group\n";
	input << "  }\n";
	input << "}\n";
	EU5::CommonCultureLoader loader;
	loader.loadCommonCultures(input);

	EXPECT_EQ("lithuanian_dialect", *loader.getLanguage("aukstaitian"));
	EXPECT_EQ("map_LIT", *loader.getColor("aukstaitian"));
	EXPECT_THAT(*loader.getCultureGroups("aukstaitian"), ElementsAre("lithuanian_group", "baltic_group"));

	EXPECT_EQ("western_baltic_dialect", *loader.getLanguage("curonian"));
	EXPECT_EQ("map_curonian", *loader.getColor("curonian"));
	EXPECT_THAT(*loader.getCultureGroups("curonian"), ElementsAre("baltic_group"));
}

TEST(EU5_CommonCultureLoaderTests, NulloptIsReturnedForMismatches)
{
	std::stringstream input;
	input << "aukstaitian = {\n";
	input << "  language = lithuanian_dialect\n";
	input << "  color = map_LIT\n";
	input << "}\n";
	EU5::CommonCultureLoader loader;
	loader.loadCommonCultures(input);

	EXPECT_EQ(std::nullopt, loader.getLanguage("nonexistent"));
	EXPECT_EQ(std::nullopt, loader.getColor("nonexistent"));
	EXPECT_EQ(std::nullopt, loader.getCultureGroups("nonexistent"));
}
