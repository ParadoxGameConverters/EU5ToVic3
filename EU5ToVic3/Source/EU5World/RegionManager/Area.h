#ifndef EU5_AREA_H
#define EU5_AREA_H
#include "Parser.h"
#include "Province.h"
#include <set>

namespace EU5
{
class Area: commonItems::parser
{
  public:
	Area() = default;
	explicit Area(std::istream& theStream);

	[[nodiscard]] bool areaContainsLocation(const std::string& theLocation) const;
	[[nodiscard]] const auto& getProvinces() const { return provinces; }

  private:
	void registerKeys();

	std::map<std::string, std::shared_ptr<Province>> provinces;
};
} // namespace EU5

#endif // EU5_AREA_H