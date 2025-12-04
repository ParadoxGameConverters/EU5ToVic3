#ifndef EU5_LOCATION_H
#define EU5_LOCATION_H

namespace EU5
{
class Location
{
  public:
	Location() = default;
	explicit Location(int theLocationID, std::string theLocationName);

	[[nodiscard]] int getID() const { return locID; }
	[[nodiscard]] const auto& getName() const { return name; }

  private:
	int locID = 0;
	std::string name;
};
} // namespace EU5

#endif // EU5_LOCATION_H
