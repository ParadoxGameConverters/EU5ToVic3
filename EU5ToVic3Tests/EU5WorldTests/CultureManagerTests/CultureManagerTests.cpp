#include "CultureManager/CultureManager.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>

TEST(EU5_CultureManagerTests, primitivesDefaultToBlank)
{
	const EU5::CultureManager manager;

	EXPECT_TRUE(manager.getCultures().empty());
}

TEST(EU5_CultureManagerTests, CulturesCanBeLoaded)
{
	std::stringstream input;
	input << "database = {\n";
	input << "  0 = { name=\"dakelh_culture\" culture_definition=dakelh_culture color=rgb {68 252 27 } language=nadene_language }\n";
	input << "  1 = { name=\"wetsuweten_culture\" culture_definition=wetsuweten_culture color=rgb {105 220 125 } language=nadene_language }\n";
	input << "  2 = { name=\"sekani_culture\" culture_definition=sekani_culture color=rgb {13 130 95 } language=nadene_language }\n";
	input << "}\n";
	EU5::CultureManager manager;
	manager.loadCultures(input);

	EXPECT_EQ(3, manager.getCultures().size());
	EXPECT_EQ("dakelh_culture", manager.getCultureByID(0)->getName());
	EXPECT_EQ("wetsuweten_culture", manager.getCultureByID(1)->getName());
	EXPECT_EQ("sekani_culture", manager.getCultureByID(2)->getName());
}

TEST(EU5_CultureManagerTests, MissingCultureReturnsNullptr)
{
	const EU5::CultureManager manager;

	EXPECT_EQ(nullptr, manager.getCultureByID(99));
}
