#ifndef PROVINCE_MANAGER
#define PROVINCE_MANAGER
#include "EU5Location.h"
#include <map>

namespace EU5
{
class ProvinceManager: commonItems::parser
{
  public:
	ProvinceManager() = default;

	void loadProvinces(std::istream& theStream);
	void registerLocation(int theLocationID, const std::string& locationName);

	[[nodiscard]] const auto& getSeenLocations() const { return seenLocations; }
	[[nodiscard]] const auto& getAllLocations() const { return locations; }
	[[nodiscard]] std::shared_ptr<Location> getSeenLocationByID(int theID) const;

  private:
	void registerKeys();

	std::map<std::string, std::shared_ptr<Location>> seenLocations;

	std::map<std::string, std::shared_ptr<Location>> locations;				// Important ones.
	std::map<std::string, std::shared_ptr<Location>> wastelands;			// Wastelands we DO care for but can't use as we'd use regular Locations.
	std::map<std::string, std::shared_ptr<Location>> discardedLocations; // RNW, lakes etc. Stuff we know about but don't care for.
};
} // namespace EU5

#endif // PROVINCE_MANAGER