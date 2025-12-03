#ifndef EU5_PROVINCE_H
#define EU5_PROVINCE_H
#include "Parser.h"
#include <set>

namespace EU5
{
class Province: commonItems::parser
{
  public:
	Province() = default;
	explicit Province(std::istream& theStream);

	[[nodiscard]] const auto& getLocations() const { return locations; }
	[[nodiscard]] bool provinceContainsLocation(const std::string& location) const;

  private:
	void registerKeys();

	std::set<std::string> locations;
};
} // namespace EU4

#endif // EU5_PROVINCE_H