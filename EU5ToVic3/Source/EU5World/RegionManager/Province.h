#ifndef EU5_PROVINCE_H
#define EU5_PROVINCE_H
#include <set>

namespace EU5
{
class Province
{
  public:
	Province() = default;
	explicit Province(const std::vector<std::string>& theLocations);

	[[nodiscard]] const auto& getLocations() const { return locations; }
	[[nodiscard]] bool provinceContainsLocation(const std::string& location) const;

  private:
	void registerKeys();

	std::set<std::string> locations;
};
} // namespace EU4

#endif // EU5_PROVINCE_H