#ifndef EU5_REGION_H
#define EU5_REGION_H
#include "Area.h"
#include "Parser.h"

namespace EU5
{
class Region: commonItems::parser
{
  public:
	Region() = default;
	explicit Region(std::istream& theStream);

	[[nodiscard]] bool regionContainsLocation(const std::string& theLocation) const;
	[[nodiscard]] const auto& getAreas() const { return areas; }

  private:
	void registerKeys();

	std::map<std::string, std::shared_ptr<Area>> areas;
};
} // namespace EU5

#endif // EU5_REGION_H