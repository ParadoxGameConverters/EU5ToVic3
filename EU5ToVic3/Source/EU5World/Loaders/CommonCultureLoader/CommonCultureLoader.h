#ifndef COMMON_CULTURE_LOADER_H
#define COMMON_CULTURE_LOADER_H
#include "ModLoader/ModFilesystem.h"
#include "Parser.h"
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace EU5
{
class CommonCultureLoader: commonItems::parser
{
  public:
	CommonCultureLoader() = default;

	void loadCommonCultures(const commonItems::ModFilesystem& modFS);
	void loadCommonCultures(std::istream& theStream);

	[[nodiscard]] std::optional<std::string> getLanguage(const std::string& cultureName) const;
	[[nodiscard]] std::optional<std::string> getColor(const std::string& cultureName) const;
	[[nodiscard]] std::optional<std::vector<std::string>> getCultureGroups(const std::string& cultureName) const;

  private:
	void registerKeys();

	std::map<std::string, std::string> cultureLanguages;
	std::map<std::string, std::string> cultureColors;
	std::map<std::string, std::vector<std::string>> cultureGroups;
};
} // namespace EU5

#endif // COMMON_CULTURE_LOADER_H
