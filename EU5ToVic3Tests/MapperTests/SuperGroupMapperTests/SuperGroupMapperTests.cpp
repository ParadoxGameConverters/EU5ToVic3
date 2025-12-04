#include "Mappers/SuperGroupMapper/SuperGroupMapper.h"
#include "gtest/gtest.h"

TEST(Mappers_SuperGroupMapperTests, superGroupsCanBeLoadedAndPinged)
{
	std::stringstream input;
	input << "old_world = { continents = {europe asia africa} }\n";
	input << "new_world = { continents = {america oceania} }\n";
	mappers::SuperGroupMapper superGroupMapper;
	superGroupMapper.loadSuperGroups(input);

	EXPECT_EQ("old_world", *superGroupMapper.getGroupForContinent("asia"));
	EXPECT_EQ("new_world", *superGroupMapper.getGroupForContinent("america"));
}

TEST(Mappers_SuperGroupMapperTests, superGroupNameMismatchReturnsNullopt)
{
	std::stringstream input;
	input << "old_world = { continents = {europe asia africa} }\n";
	input << "new_world = { continents = {america oceania} }\n";
	mappers::SuperGroupMapper superGroupMapper;
	superGroupMapper.loadSuperGroups(input);

	EXPECT_EQ(std::nullopt, superGroupMapper.getGroupForContinent("nonsense"));
}
