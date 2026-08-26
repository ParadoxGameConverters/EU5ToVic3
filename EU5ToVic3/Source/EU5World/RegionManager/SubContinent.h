#ifndef EU5_SUBCONTINENT_H
#define EU5_SUBCONTINENT_H
#include "Region.h"

namespace EU5
{
class SubContinent: commonItems::parser
{
  public:
	SubContinent() = default;
	explicit SubContinent(std::istream& theStream);

	[[nodiscard]] const auto& getRegions() const { return regions; }
	[[nodiscard]] bool subContinentContainsLocation(const std::string& theLocation) const;

  private:
	void registerKeys();

	std::map<std::string, std::shared_ptr<Region>> regions;
};
} // namespace EU5

#endif // EU5_SUBCONTINENT_H
