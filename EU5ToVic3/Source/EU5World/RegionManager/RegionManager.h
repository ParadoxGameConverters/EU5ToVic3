#ifndef EU5_REGIONMANAGER_H
#define EU5_REGIONMANAGER_H
#include "Area.h"
#include "Continent.h"
#include "Mappers/SuperGroupMapper/SuperGroupMapper.h"
#include "Parser.h"
#include "Province.h"
#include "Region.h"
#include "SuperRegion.h"

namespace EU5
{
class ProvinceManager;
class RegionManager: commonItems::parser
{
  public:
	void loadRegions(const commonItems::ModFilesystem& modsFS);
	void loadRegions(std::istream& theStream); // for testing

	[[nodiscard]] bool locationIsInRegion(const std::string& location, const std::string& regionName) const;
	[[nodiscard]] bool regionNameIsValid(const std::string& regionName) const;
	[[nodiscard]] bool locationIsValid(const std::string& location) const;

	[[nodiscard]] std::optional<std::string> getParentAreaName(const std::string& location) const;
	[[nodiscard]] std::optional<std::string> getParentProvinceName(const std::string& location) const;
	[[nodiscard]] std::optional<std::string> getParentRegionName(const std::string& location) const;
	[[nodiscard]] std::optional<std::string> getParentSuperRegionName(const std::string& location) const;
	[[nodiscard]] std::optional<std::string> getParentContinentName(const std::string& location) const;
	[[nodiscard]] std::optional<std::string> getParentSuperGroupName(const std::string& location) const;

	void loadSuperGroups(const mappers::SuperGroupMapper& sgMapper) { superGroupMapper = sgMapper; }
	void applySuperGroups();

  private:
	void registerKeys();

	std::map<std::string, std::shared_ptr<Province>> provinces;
	std::map<std::string, std::shared_ptr<Area>> areas;
	std::map<std::string, std::shared_ptr<Region>> regions;
	std::map<std::string, std::shared_ptr<SuperRegion>> superRegions;
	std::map<std::string, std::shared_ptr<Continent>> continents;

	mappers::SuperGroupMapper superGroupMapper;
};
} // namespace EU5

#endif // EU5_REGIONMANAGER_H
