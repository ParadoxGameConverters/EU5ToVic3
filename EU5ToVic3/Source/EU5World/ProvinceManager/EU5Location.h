#ifndef EU5_LOCATION_H
#define EU5_LOCATION_H

namespace EU5
{
class Location
{
  public:
	Location() = default;
	explicit Location(int theLocationID, std::string theSystemName);

	[[nodiscard]] int getLocationID() const { return locID; }
	[[nodiscard]] const auto& getLocationSystemName() const { return systemName; }

  private:
	int locID = 0;
	std::string systemName;
};
} // namespace EU5

#endif // EU5_LOCATION_H
