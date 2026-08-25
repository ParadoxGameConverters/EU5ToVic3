#ifndef EU5_LOCATION_H
#define EU5_LOCATION_H

namespace EU5
{
struct EstateTax
{
	double tax = 0;
	double possibleTax = 0;
	bool operator==(const EstateTax&) const = default;
};

struct Integration
{
	// core, integrated, or conquered (might be more)
	std::string type;
	double progress = 0;
	bool operator==(const Integration&) const = default;
};

class Location: commonItems::parser
{
  public:
	Location() = default;
	explicit Location(int theLocationID, std::string theLocationName);

	void parseData(std::istream& theStream);

	[[nodiscard]] int getID() const { return locID; }
	[[nodiscard]] int getOwnerID() const { return ownerID; }
	[[nodiscard]] int getControllerID() const { return controllerID; }
	[[nodiscard]] int getPreviousOwnerID() const { return previousOwnerID; }
	[[nodiscard]] int getPreviousControllerID() const { return previousControllerID; }
	[[nodiscard]] double getDevelopment() const { return development; }
	[[nodiscard]] double getProsperity() const { return prosperity; }
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getLanguage() const { return language; }
	[[nodiscard]] const auto& getDialect() const { return dialect; }
	[[nodiscard]] const auto& getRank() const { return rank; }
	[[nodiscard]] const auto& getOriginalRank() const { return originalRank; }
	[[nodiscard]] const auto& getRawMaterial() const { return rawMaterial; }
	[[nodiscard]] const auto& getCountriesWithCore() const { return countriesWithCore; }

	[[nodiscard]] int getReligionID() const { return religionID; }
	[[nodiscard]] int getCultureID() const { return cultureID; }
	[[nodiscard]] int getSecondaryCultureID() const { return secondaryCultureID; }
	[[nodiscard]] double getCulturalUnity() const { return culturalUnity; }

	[[nodiscard]] const auto& getLastOwnerChange() const { return lastOwnerChange; }
	[[nodiscard]] const auto& getLastControllerChange() const { return lastControllerChange; }

	[[nodiscard]] int getMaxRawMaterialWorkers() const { return maxRawMaterialWorkers; }
	[[nodiscard]] double getControl() const { return control; }
	[[nodiscard]] bool getCapControl() const { return capControl; }
	[[nodiscard]] bool getRoadToCapital() const { return roadToCapital; }
	[[nodiscard]] double getProximity() const { return proximity; }
	[[nodiscard]] double getLocalProximityPropagation() const { return localProximityPropagation; }
	[[nodiscard]] double getValueFlow() const { return valueFlow; }

	[[nodiscard]] int getProvinceID() const { return provinceID; }
	[[nodiscard]] double getTax() const { return tax; }
	[[nodiscard]] double getPossibleTax() const { return possibleTax; }
	[[nodiscard]] const auto& getEstateTax() const { return estateTax; }
	[[nodiscard]] const auto& getInstitutions() const { return institutions; }

	[[nodiscard]] const auto& getUnitIDs() const { return unitIDs; }
	[[nodiscard]] const auto& getPortIDs() const { return portIDs; }
	[[nodiscard]] const auto& getPopIDs() const { return popIDs; }

	[[nodiscard]] double getGarrison() const { return garrison; }
	[[nodiscard]] const auto& getCounters() const { return counters; }

	[[nodiscard]] int getMarketID() const { return marketID; }
	[[nodiscard]] int getSecondBestMarketID() const { return secondBestMarketID; }
	[[nodiscard]] int getMarketParentID() const { return marketParentID; }
	[[nodiscard]] double getMarketAccess() const { return marketAccess; }
	[[nodiscard]] double getMarketAttraction() const { return marketAttraction; }
	[[nodiscard]] double getSecondBestMarketAccess() const { return secondBestMarketAccess; }

	[[nodiscard]] const auto& getIntegrationData() const { return integrationData; }

  private:
	void registerKeys();

	int locID = 0;
	std::string name;

	int ownerID = 0;
	int controllerID = 0;
	int previousOwnerID = 0;
	int previousControllerID = 0;
	double development = 0;
	double prosperity = 0;

	std::string language;
	std::string dialect;
	std::string rank;
	std::string originalRank;
	std::string rawMaterial;

	// country id
	std::set<int> countriesWithCore;

	int religionID = 0;
	int cultureID = 0;
	int secondaryCultureID = 0;
	double culturalUnity = 0;

	date lastOwnerChange;
	date lastControllerChange;

	int maxRawMaterialWorkers = 0;
	double control = 0;
	bool capControl = false;
	bool roadToCapital = false;
	double proximity = 0;
	double localProximityPropagation = 0;
	double valueFlow = 0;

	int provinceID = 0;
	// TODO figure out how economy.income is calculated from shtuff like this
	double tax = 0;
	double possibleTax = 0;
	// estate key -> tax, possibleTax
	std::map<std::string, EstateTax> estateTax;

	// instritution name -> 0.0001 - 100 representing spread %
	std::map<std::string, double> institutions;

	std::vector<int> unitIDs;
	std::vector<int> portIDs; // list in save but I only ever see one value?
	// TODO figure out what to do with location-specific pop info that isn't tied to specific pop IDs
	//      e.g. unemployment, rgo employment, culture/religion conversion
	std::vector<int> popIDs;

	double garrison = 0;
	std::map<std::string, int> counters;

	int marketID = 0;
	int secondBestMarketID = 0;
	int marketParentID = 0;
	double marketAccess = 0;
	double marketAttraction = 0;
	double secondBestMarketAccess = 0;

	// country id -> type, progress
	std::map<int, Integration> integrationData;

	// TODO figure out what these fields are:
	//      ub, flip_sources
	//      ub in my test saves are all no...
};
} // namespace EU5

#endif // EU5_LOCATION_H
