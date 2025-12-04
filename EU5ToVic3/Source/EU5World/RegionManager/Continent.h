#ifndef EU5_CONTINENT_H
#define EU5_CONTINENT_H
#include "SuperRegion.h"

namespace EU5
{
class Continent: commonItems::parser
{
  public:
	Continent() = default;
	explicit Continent(std::istream& theStream);

	[[nodiscard]] const auto& getSuperRegions() const { return superRegions; }
	[[nodiscard]] const auto& getSuperGroup() const { return superGroup; }
	[[nodiscard]] bool continentContainsLocation(const std::string& location) const;

	void setSuperGroup(const std::string& superGroupName) { superGroup = superGroupName; }

  private:
	void registerKeys();

	std::map<std::string, std::shared_ptr<SuperRegion>> superRegions;
	std::string superGroup;
};
} // namespace EU5

#endif // EU5_CONTINENT_H