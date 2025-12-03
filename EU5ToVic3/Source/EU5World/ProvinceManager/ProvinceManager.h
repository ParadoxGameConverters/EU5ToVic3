#ifndef PROVINCE_MANAGER
#define PROVINCE_MANAGER
#include "EU5Location.h"
#include <map>


namespace EU5
{
class ProvinceManager
{
  public:
	ProvinceManager() = default;

	void registerLocation(int theLocationID, const std::string& theSystemName);
	[[nodiscard]] const auto& getSeenLocations() const { return seenLocations; }
	[[nodiscard]] const std::shared_ptr<Location>& getSeenLocationByID(int theID) const;

  private:
	std::map<std::string, std::shared_ptr<Location>> seenLocations;

	std::map<int, std::shared_ptr<Location>> locations;			 // Important ones.
	std::map<int, std::shared_ptr<Location>> wastelands;			 // Wastelands we DO care for but can't use as we'd use regular Locations.
	std::map<int, std::shared_ptr<Location>> discardedLocations; // RNW, lakes etc. Stuff we know about but don't care for.
};
} // namespace EU5

#endif // PROVINCE_MANAGER