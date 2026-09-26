#ifndef BUILDING_MANAGER_H
#define BUILDING_MANAGER_H
#include "EU5Building.h"
#include "Parser.h"
#include <map>
#include <memory>
#include <vector>

namespace EU5
{
class BuildingManager: commonItems::parser
{
  public:
	BuildingManager() = default;

	void loadBuildings(std::istream& theStream);

	[[nodiscard]] const auto& getBuildings() const { return buildings; }
	[[nodiscard]] std::shared_ptr<Building> getBuildingByID(int theBuildingID) const;
	[[nodiscard]] std::vector<int> getBuildingIDsAtLocation(int theLocationID) const;

  private:
	void registerKeys();

	std::map<int, std::shared_ptr<Building>> buildings;
	std::map<int, std::vector<int>> buildingIDsByLocation;
	parser buildingDatabaseParser;
};
} // namespace EU5

#endif // BUILDING_MANAGER_H
