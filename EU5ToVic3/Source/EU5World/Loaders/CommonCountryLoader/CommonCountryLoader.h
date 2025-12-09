#ifndef COMMON_COUNTRY_LOADER
#define COMMON_COUNTRY_LOADER
#include "Color.h"
#include "ModLoader/ModFilesystem.h"
#include "Parser.h"

namespace EU5
{
class CommonCountryLoader: commonItems::parser
{
  public:
	CommonCountryLoader() = default;

	void loadCommonCountries(const commonItems::ModFilesystem& modFS);

	[[nodiscard]] std::optional<commonItems::Color> getColor1(const std::string& tag) const;
	[[nodiscard]] std::optional<commonItems::Color> getColor2(const std::string& tag) const;
	[[nodiscard]] std::optional<std::string> getCountryCulture(const std::string& tag) const;
	[[nodiscard]] std::optional<std::string> getCountryReligion(const std::string& tag) const;

  private:
	void registerKeys();

	parser tagParser;

	std::map<std::string, commonItems::Color> countryColors1;
	std::map<std::string, commonItems::Color> countryColors2;
	std::map<std::string, std::string> countryCulture;
	std::map<std::string, std::string> countryReligion;
	std::string currentTag;
};
} // namespace EU5

#endif // COMMON_COUNTRY_LOADER