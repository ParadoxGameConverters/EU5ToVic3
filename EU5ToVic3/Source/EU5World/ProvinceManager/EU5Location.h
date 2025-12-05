#ifndef EU5_LOCATION_H
#define EU5_LOCATION_H

namespace EU5
{
class Location: commonItems::parser
{
  public:
	Location() = default;
	explicit Location(int theLocationID, std::string theLocationName);

	void parseData(std::istream& theStream);

	[[nodiscard]] int getID() const { return locID; }
	[[nodiscard]] int getOwnerID() const { return ownerID; }
	[[nodiscard]] int getControllerID() const { return controllerID; }
	[[nodiscard]] double getDevelopment() const { return development; }
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getLanguage() const { return language; }
	[[nodiscard]] const auto& getDialect() const { return dialect; }
	[[nodiscard]] const auto& getRank() const { return rank; }
	[[nodiscard]] const auto& getCoreIDs() const { return coreIDs; }

  private:
	void registerKeys();

	int locID = 0;
	std::string name;

	int ownerID = 0;
	int controllerID = 0;
	double development = 0;

	std::string language;
	std::string dialect;
	std::string rank;

	std::set<int> coreIDs;
};
} // namespace EU5

#endif // EU5_LOCATION_H
