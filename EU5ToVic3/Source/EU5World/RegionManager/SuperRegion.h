#ifndef EU5_SUPERREGION_H
#define EU5_SUPERREGION_H
#include "Region.h"

namespace EU5
{
class SuperRegion: commonItems::parser
{
  public:
	SuperRegion() = default;
	explicit SuperRegion(std::istream& theStream);

	[[nodiscard]] const auto& getRegions() const { return regions; }
	[[nodiscard]] bool superRegionContainsLocation(const std::string& theLocation) const;

  private:
	void registerKeys();

	std::map<std::string, std::shared_ptr<Region>> regions;
};
} // namespace EU5

#endif // EU5_SUPERREGION_H