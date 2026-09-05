#ifndef BUILDING_TYPE_LOADER_H
#define BUILDING_TYPE_LOADER_H
#include "ModLoader/ModFilesystem.h"
#include "Parser.h"
#include "ProductionMethod.h"
#include <map>
#include <optional>
#include <string>

namespace EU5
{
class BuildingTypeLoader: commonItems::parser
{
  public:
	BuildingTypeLoader() = default;

	void loadBuildingTypes(const commonItems::ModFilesystem& modFS);
	void loadBuildingTypes(std::istream& theStream);

	[[nodiscard]] std::optional<std::string> getPopType(const std::string& buildingType) const;
	[[nodiscard]] std::optional<ProductionMethod> getProductionMethod(const std::string& productionMethodKey) const;

  private:
	void registerKeys();

	std::map<std::string, std::string> popTypes;
	std::map<std::string, ProductionMethod> productionMethods;
};
} // namespace EU5

#endif // BUILDING_TYPE_LOADER_H
