#ifndef EU5_AREA_H
#define EU5_AREA_H
#include "Parser.h"
#include <set>

namespace EU5
{
class Area: commonItems::parser
{
  public:
	Area() = default;
	explicit Area(std::istream& theStream);

	[[nodiscard]] const auto& getProvinces() const { return provinces; }
	[[nodiscard]] bool areaContainsProvince(int province) const;

  private:
	void registerKeys();

	std::set<int> provinces;
};
} // namespace EU5

#endif // EU5_AREA_H