#ifndef EU5_BUILDING_H
#define EU5_BUILDING_H
#include "Parser.h"
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
	[[nodiscard]] int getPopID() const { return popID; }
	[[nodiscard]] double getEmployed() const { return employed; }
	[[nodiscard]] int getEmploymentRequirement() const { return employmentRequirement; }
	[[nodiscard]] const auto& getEmploymentRequirementStatus() const { return employmentRequirementStatus; }
	[[nodiscard]] int getEstablishmentProgress() const { return establishmentProgress; }
	[[nodiscard]] double getLastMonthsProfit() const { return lastMonthsProfit; }
	[[nodiscard]] double getUpkeep() const { return upkeep; }
	[[nodiscard]] bool getOpen() const { return open; }
	[[nodiscard]] bool getSubsidized() const { return subsidized; }
	[[nodiscard]] const auto& getProductionMethod() const { return productionMethod; }
	[[nodiscard]] double getInput() const { return input; }

  private:
	void registerKeys();

	int buildingID = 0;
	std::string type;
	int level = 0;
	int locationID = 0;
	int ownerEstateID = 0;
	int popID = 0;
	double employed = 0;
	int employmentRequirement = 0;
	std::string employmentRequirementStatus;
	int establishmentProgress = 0;
	double lastMonthsProfit = 0;
	double upkeep = 0;
	bool open = false;
	bool subsidized = false;
	std::string productionMethod;
	double input = 0;
};
} // namespace EU5

#endif // EU5_BUILDING_H
