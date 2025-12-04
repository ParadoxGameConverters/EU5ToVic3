#include "ProvinceManager/ProvinceManager.h"
#include "RegionManager/RegionManager.h"
#include "gtest/gtest.h"

// This is a collective mapper test for Area, Region, SuperRegion and RegionManager.

TEST(Mappers_RegionMapperTests, regionMapperCanBeEnabled)
{
	// We start humble, it's a machine.
	EU5::RegionManager theMapper;

	std::stringstream input;
	theMapper.loadRegions(input);
	EXPECT_FALSE(theMapper.locationIsInRegion("location", "region"));
	EXPECT_FALSE(theMapper.regionNameIsValid("region"));
	EXPECT_FALSE(theMapper.getParentProvinceName("location"));
	EXPECT_FALSE(theMapper.getParentAreaName("location"));
	EXPECT_FALSE(theMapper.getParentRegionName("location"));
	EXPECT_FALSE(theMapper.getParentSuperRegionName("location"));
	EXPECT_FALSE(theMapper.getParentContinentName("location"));
	EXPECT_FALSE(theMapper.getParentSuperGroupName("location"));
}

TEST(Mappers_RegionMapperTests, locationServicesWork)
{
	EU5::RegionManager theMapper;
	std::stringstream input;
	input << "europe = { western_europe = { scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm "
				"tierp heby } } "
				"} } }\n";
	input << "oceania = { australasia = { australia_region = { southwestern_australia_area = { wardandi_province = {wardandi gurbillup nannup manjimup "
				"bibbulman jarrah_karri munite mallee kaniyang "
				"} } } } }\n";
	theMapper.loadRegions(input);

	EXPECT_TRUE(theMapper.locationIsInRegion("mallee", "wardandi_province"));
	EXPECT_TRUE(theMapper.locationIsInRegion("mallee", "southwestern_australia_area"));
	EXPECT_TRUE(theMapper.locationIsInRegion("mallee", "australia_region"));
	EXPECT_TRUE(theMapper.locationIsInRegion("mallee", "australasia"));
	EXPECT_TRUE(theMapper.locationIsInRegion("mallee", "oceania"));

	EXPECT_TRUE(theMapper.locationIsValid("mallee"));
}

TEST(Mappers_RegionMapperTests, locationServicesCorrectlyFail)
{
	EU5::RegionManager theMapper;
	std::stringstream input;
	input << "europe = { western_europe = { scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm "
				"tierp heby } } "
				"} } }\n";
	input << "oceania = { australasia = { australia_region = { southwestern_australia_area = { wardandi_province = {wardandi gurbillup nannup manjimup "
				"bibbulman jarrah_karri munite mallee kaniyang "
				"} } } } }\n";
	theMapper.loadRegions(input);

	EXPECT_FALSE(theMapper.locationIsInRegion("mallee", "uppland_province"));
	EXPECT_FALSE(theMapper.locationIsInRegion("mallee", "svealand_area"));
	EXPECT_FALSE(theMapper.locationIsInRegion("mallee", "scandinavian_region"));
	EXPECT_FALSE(theMapper.locationIsInRegion("mallee", "western_europe"));
	EXPECT_FALSE(theMapper.locationIsInRegion("mallee", "europe"));
	EXPECT_FALSE(theMapper.locationIsInRegion("mallee", "nonsense"));
}

TEST(Mappers_RegionMapperTests, locationServicesFailForNonsense)
{
	EU5::RegionManager theMapper;
	std::stringstream input;
	input << "europe = { western_europe = { scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm "
				"tierp heby } } "
				"} } }\n";
	input << "oceania = { australasia = { australia_region = { southwestern_australia_area = { wardandi_province = {wardandi gurbillup nannup manjimup "
				"bibbulman jarrah_karri munite mallee kaniyang "
				"} } } } }\n";
	theMapper.loadRegions(input);

	EXPECT_FALSE(theMapper.locationIsInRegion("mallee", "nonsense"));
	EXPECT_FALSE(theMapper.locationIsInRegion("nonsense", "europe"));

	EXPECT_FALSE(theMapper.locationIsValid("nonsense"));
}

TEST(Mappers_RegionMapperTests, correctParentLocationsReported)
{
	EU5::RegionManager theMapper;
	std::stringstream input;
	input << "europe = { western_europe = { scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm "
				"tierp heby } } "
				"} } }\n";
	input << "oceania = { australasia = { australia_region = { southwestern_australia_area = { wardandi_province = {wardandi gurbillup nannup manjimup "
				"bibbulman jarrah_karri munite mallee kaniyang "
				"} } } } }\n";
	theMapper.loadRegions(input);

	EXPECT_EQ("uppland_province", *theMapper.getParentProvinceName("norrtalje"));
	EXPECT_EQ("svealand_area", *theMapper.getParentAreaName("norrtalje"));
	EXPECT_EQ("scandinavian_region", *theMapper.getParentRegionName("norrtalje"));
	EXPECT_EQ("western_europe", *theMapper.getParentSuperRegionName("norrtalje"));
	EXPECT_EQ("europe", *theMapper.getParentContinentName("norrtalje"));
	EXPECT_EQ("wardandi_province", *theMapper.getParentProvinceName("nannup"));
	EXPECT_EQ("southwestern_australia_area", *theMapper.getParentAreaName("nannup"));
	EXPECT_EQ("australia_region", *theMapper.getParentRegionName("nannup"));
	EXPECT_EQ("australasia", *theMapper.getParentSuperRegionName("nannup"));
	EXPECT_EQ("oceania", *theMapper.getParentContinentName("nannup"));
}

TEST(Mappers_RegionMapperTests, wrongParentLocationsReturnNullopt)
{
	EU5::RegionManager theMapper;
	std::stringstream input;
	input << "europe = { western_europe = { scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm "
				"tierp heby } } "
				"} } }\n";
	input << "oceania = { australasia = { australia_region = { southwestern_australia_area = { wardandi_province = {wardandi gurbillup nannup manjimup "
				"bibbulman jarrah_karri munite mallee kaniyang "
				"} } } } }\n";
	theMapper.loadRegions(input);

	EXPECT_FALSE(theMapper.getParentProvinceName("nonsense"));
	EXPECT_FALSE(theMapper.getParentAreaName("nonsense"));
	EXPECT_FALSE(theMapper.getParentRegionName("nonsense"));
	EXPECT_FALSE(theMapper.getParentSuperRegionName("nonsense"));
	EXPECT_FALSE(theMapper.getParentContinentName("nonsense"));
}

TEST(Mappers_RegionMapperTests, locationNameValidationWorks)
{
	EU5::RegionManager theMapper;
	std::stringstream input;
	input << "europe = { western_europe = { scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm "
				"tierp heby } } "
				"} } }\n";
	input << "oceania = { australasia = { australia_region = { southwestern_australia_area = { wardandi_province = {wardandi gurbillup nannup manjimup "
				"bibbulman jarrah_karri munite mallee kaniyang "
				"} } } } }\n";
	theMapper.loadRegions(input);

	EXPECT_TRUE(theMapper.regionNameIsValid("uppland_province"));
	EXPECT_TRUE(theMapper.regionNameIsValid("svealand_area"));
	EXPECT_TRUE(theMapper.regionNameIsValid("scandinavian_region"));
	EXPECT_TRUE(theMapper.regionNameIsValid("western_europe"));
	EXPECT_TRUE(theMapper.regionNameIsValid("europe"));
	EXPECT_FALSE(theMapper.regionNameIsValid("nonsense"));
}

TEST(Mappers_RegionMapperTests, superGroupsCanBeAssignedAndPinged)
{
	EU5::RegionManager theMapper;
	std::stringstream input;
	input << "europe = { western_europe = { scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm "
				"tierp heby } } "
				"} } }\n";
	input << "oceania = { australasia = { australia_region = { southwestern_australia_area = { wardandi_province = {wardandi gurbillup nannup manjimup "
				"bibbulman jarrah_karri munite mallee kaniyang "
				"} } } } }\n";
	theMapper.loadRegions(input);

	EXPECT_EQ("old_world", *theMapper.getParentSuperGroupName("stockholm"));
	EXPECT_EQ("new_world", *theMapper.getParentSuperGroupName("nannup"));
}

TEST(Mappers_RegionMapperTests, supergroupMismatchReturnsNullopt)
{
	EU5::RegionManager theMapper;
	std::stringstream input;
	input << "europe = { western_europe = { scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm "
				"tierp heby } } "
				"} } }\n";
	input << "oceania = { australasia = { australia_region = { southwestern_australia_area = { wardandi_province = {wardandi gurbillup nannup manjimup "
				"bibbulman jarrah_karri munite mallee kaniyang "
				"} } } } }\n";
	theMapper.loadRegions(input);

	EXPECT_EQ(std::nullopt, theMapper.getParentSuperGroupName("nonsense"));
}

TEST(Mappers_RegionMapperTests, brokenAndMissingSuperGroupDefaultsToIgnored)
{
	EU5::RegionManager theMapper;
	std::stringstream input;
	input << "europe = { western_europe = { scandinavian_region = { svealand_area = { uppland_province = { stockholm norrtalje enkoping uppsala kastelholm "
				"tierp heby } } "
				"} } }\n";
	input << "oceania = { australasia = { australia_region = { southwestern_australia_area = { wardandi_province = {wardandi gurbillup nannup manjimup "
				"bibbulman jarrah_karri munite mallee kaniyang "
				"} } } } }\n";
	theMapper.loadRegions(input);

	std::stringstream superGroupStream;
	superGroupStream << "old_world = { continentsBROKE = { europe asia africa } }\n"; // missing new world, broken old world
	mappers::SuperGroupMapper superGroupMapper;
	superGroupMapper.loadSuperGroups(superGroupStream);

	theMapper.loadSuperGroups(superGroupMapper);
	theMapper.applySuperGroups();

	EXPECT_EQ("this_is_ignored", theMapper.getParentSuperGroupName("stockholm"));
	EXPECT_EQ("this_is_ignored", theMapper.getParentSuperGroupName("wardandi"));
}
