#include "Mappers/SuperGroupMapper/SuperGroupParser.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
using testing::ElementsAre;
using testing::Pair;
using testing::UnorderedElementsAre;

TEST(Mappers_SuperGroupParserTests, PrimitivesDefaultToEmpty)
{
	std::stringstream input;
	const auto superGroup = mappers::SuperGroupParser(input);

	EXPECT_TRUE(superGroup.getContinentNames().empty());
}

TEST(Mappers_SuperGroupParserTests, SuperGroupCanBeLoaded)
{
	std::stringstream input;
	input << "continents = { europe africa }\n";
	const auto superRegion = mappers::SuperGroupParser(input);

	EXPECT_THAT(superRegion.getContinentNames(), UnorderedElementsAre("africa", "europe"));
}
