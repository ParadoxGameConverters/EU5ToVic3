#ifndef EU5_BUILDING_H
#define EU5_BUILDING_H
#include "Parser.h"
#include <optional>
#include <set>
#include <string>

namespace EU5
{
class Building: commonItems::parser
{
  public:
	Building() = default;
	Building(int theBuildingID, std::istream& theStream);

	[[nodiscard]] int getID() const { return buildingID; }
	[[nodiscard]] const auto& getType() const { return type; }
	[[nodiscard]] int getLevel() const { return level; }
	[[nodiscard]] int getLocationID() const { return locationID; }
	[[nodiscard]] int getOwnerEstateID() const { return ownerEstateID; }
	[[nodiscard]] std::optional<int> getPopID() const { return popID; }
	[[nodiscard]] double getEmployed() const { return employed; }
	[[nodiscard]] int getEmploymentRequirement() const { return employmentRequirement; }
	[[nodiscard]] const auto& getEmploymentRequirementStatus() const { return employmentRequirementStatus; }
	[[nodiscard]] int getEstablishmentProgress() const { return establishmentProgress; }
	[[nodiscard]] double getLastMonthsProfit() const { return lastMonthsProfit; }
	[[nodiscard]] double getUpkeep() const { return upkeep; }
	[[nodiscard]] bool getOpen() const { return open; }
	[[nodiscard]] bool getSubsidized() const { return subsidized; }
	[[nodiscard]] const auto& getProductionMethods() const { return productionMethods; }

  private:
	void registerKeys();

	int buildingID = 0;
	std::string type;
	int level = 0;
	int locationID = 0;
	int ownerEstateID = 0;
	std::optional<int> popID;
	double employed = 0;
	int employmentRequirement = 100;
	std::string employmentRequirementStatus;
	int establishmentProgress = 0;
	double lastMonthsProfit = 0;
	double upkeep = 0;
	bool open = true;
	bool subsidized = false;
	std::set<std::string> productionMethods;
};
} // namespace EU5

#endif // EU5_BUILDING_H
