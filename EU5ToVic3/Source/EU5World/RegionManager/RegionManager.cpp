#include "RegionManager.h"
#include "Log.h"
#include "ProvinceManager/ProvinceManager.h"
#include <filesystem>
#include <fstream>
#include <ranges>

void EU5::RegionManager::loadRegions(const commonItems::ModFilesystem& modsFS)
{
	const auto& definitionsFile = modsFS.GetActualFileLocation("in_game/map_data/definitions.txt");
	if (!definitionsFile)
		throw std::runtime_error("There is no in_game/map_data/definitions.txt!");
	std::ifstream defStream(*definitionsFile);
	if (!defStream.is_open())
		throw std::runtime_error("Could not open " + definitionsFile->string() + "!");
	registerKeys();
	parseStream(defStream);
	clearRegisteredKeywords();
	defStream.close();

	superGroupMapper.loadSuperGroups();
	applySuperGroups();
}

void EU5::RegionManager::loadRegions(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
	superGroupMapper.loadSuperGroups();
	applySuperGroups();
}

void EU5::RegionManager::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& continentName, std::istream& theStream) {
		auto newContinent = std::make_shared<Continent>(theStream);
		continents.emplace(continentName, newContinent);
		for (const auto& [superRegionName, superRegion]: newContinent->getSuperRegions())
		{
			superRegions.emplace(superRegionName, superRegion);
			for (const auto& [regionName, region]: superRegion->getRegions())
			{
				regions.emplace(regionName, region);
				for (const auto& [areaName, area]: region->getAreas())
				{
					areas.emplace(areaName, area);
					for (const auto& [provinceName, province]: area->getProvinces())
					{
						provinces.emplace(provinceName, province);
					}
				}
			}
		}
	});
}

bool EU5::RegionManager::locationIsInRegion(const std::string& location, const std::string& regionName) const
{
	if (const auto& regionItr = regions.find(regionName); regionItr != regions.end())
		return regionItr->second->regionContainsLocation(location);

	// "Regions" are such a fluid term.
	if (const auto& superRegionItr = superRegions.find(regionName); superRegionItr != superRegions.end())
		return superRegionItr->second->superRegionContainsLocation(location);

	// And sometimes they don't mean what people think they mean at all.
	if (const auto& areaItr = areas.find(regionName); areaItr != areas.end())
		return areaItr->second->areaContainsLocation(location);

	if (const auto& continentItr = continents.find(regionName); continentItr != continents.end())
		return continentItr->second->continentContainsLocation(location);

	// Case in point.
	if (const auto& provinceItr = provinces.find(regionName); provinceItr != provinces.end())
		return provinceItr->second->provinceContainsLocation(location);

	return false;
}

std::optional<std::string> EU5::RegionManager::getParentProvinceName(const std::string& location) const
{
	for (const auto& [provinceName, province]: provinces)
		if (province->provinceContainsLocation(location))
			return provinceName;

	Log(LogLevel::Warning) << "Location " << location << " has no parent province name!";
	return std::nullopt;
}

std::optional<std::string> EU5::RegionManager::getParentAreaName(const std::string& location) const
{
	for (const auto& [areaName, area]: areas)
		if (area->areaContainsLocation(location))
			return areaName;

	Log(LogLevel::Warning) << "Location " << location << " has no parent area name!";
	return std::nullopt;
}

std::optional<std::string> EU5::RegionManager::getParentRegionName(const std::string& location) const
{
	for (const auto& [regionName, region]: regions)
		if (region->regionContainsLocation(location))
			return regionName;

	Log(LogLevel::Warning) << "Location " << location << " has no parent region name!";
	return std::nullopt;
}

std::optional<std::string> EU5::RegionManager::getParentSuperRegionName(const std::string& location) const
{
	for (const auto& [superRegionName, superRegion]: superRegions)
		if (superRegion->superRegionContainsLocation(location))
			return superRegionName;

	Log(LogLevel::Warning) << "Location " << location << " has no parent superregion name!";
	return std::nullopt;
}

std::optional<std::string> EU5::RegionManager::getParentContinentName(const std::string& location) const
{
	for (const auto& [continentName, continent]: continents)
		if (continent->continentContainsLocation(location))
			return continentName;

	Log(LogLevel::Warning) << "Location " << location << " has no parent continent name!";
	return std::nullopt;
}

std::optional<std::string> EU5::RegionManager::getParentSuperGroupName(const std::string& location) const
{
	for (const auto& continent: continents | std::views::values)
		if (continent->continentContainsLocation(location))
			return continent->getSuperGroup();

	Log(LogLevel::Warning) << "Location " << location << " has no parent supergroup name!";
	return std::nullopt;
}

bool EU5::RegionManager::regionNameIsValid(const std::string& regionName) const
{
	const auto& regionItr = regions.find(regionName);
	if (regionItr != regions.end())
		return true;

	// Who knows what the mapper needs. All kinds of stuff.
	const auto& superRegionItr = superRegions.find(regionName);
	if (superRegionItr != superRegions.end())
		return true;

	// And more stuff, what's the worst that could happen?
	const auto& areaItr = areas.find(regionName);
	if (areaItr != areas.end())
		return true;

	const auto& provinceItr = provinces.find(regionName);
	if (provinceItr != provinces.end())
		return true;

	const auto& continentItr = continents.find(regionName);
	if (continentItr != continents.end())
		return true;

	return false;
}

bool EU5::RegionManager::locationIsValid(const std::string& location) const
{
	for (const auto& area: areas | std::views::values)
		if (area->areaContainsLocation(location))
			return true;
	return false;
}

void EU5::RegionManager::applySuperGroups()
{
	for (const auto& [continentName, continent]: continents)
	{
		if (const auto& superGroup = superGroupMapper.getGroupForContinent(continentName); superGroup)
		{
			continent->setSuperGroup(*superGroup);
		}
		else
		{
			Log(LogLevel::Warning) << "Continent " << continentName << " doesn't have a supergroup in world_supergroups.txt!";
			continent->setSuperGroup("this_is_ignored"); // defaulting to the safe choice.
		}
	}
}
