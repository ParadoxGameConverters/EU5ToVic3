#ifndef EU5_COUNTRY_MANAGER
#define EU5_COUNTRY_MANAGER
#include "CommonCountryLoader/CommonCountryLoader.h"
#include "EU5Country.h"
#include "EU5LocalizationLoader/EU5LocalizationLoader.h"
#include "Parser.h"
#include <map>
#include <memory>

namespace EU5
{
class CountryManager: public commonItems::parser
{
  public:
	CountryManager() = default;

	void loadCountries(std::istream& theStream);

	[[nodiscard]] const auto& getCountries() const { return countries; }
	[[nodiscard]] std::shared_ptr<Country> getCountryByID(int countryID) const;
	[[nodiscard]] const auto& getLocalizationLoader() const { return localizationLoader; }

	// the prep
	void loadLocalizations(const EU5LocalizationLoader& loader) { localizationLoader = loader; }

  private:
	void registerKeys();

	std::map<std::string, std::shared_ptr<Country>> countries;

	CommonCountryLoader commonCountryLoader;
	EU5LocalizationLoader localizationLoader;


	parser tagRegistryParser;
	parser countryDatabaseParser;
	std::map<int, std::string> tagRegistry;
	int revolutionaryTarget = 0;
};
} // namespace EU5

#endif // EU5_COUNTRY_MANAGER