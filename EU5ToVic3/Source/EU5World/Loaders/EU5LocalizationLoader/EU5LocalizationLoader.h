#ifndef EU5_LOCALIZATION_LOADER
#define EU5_LOCALIZATION_LOADER
#include "ModLoader/ModLoader.h"
#include <map>
#include <optional>
#include <string>

class Configuration;
namespace EU5
{
typedef struct LocBlock
{
	std::string braz_por;
	std::string english;
	std::string french;
	std::string german;
	std::string japanese;
	std::string korean;
	std::string polish;
	std::string russian;
	std::string simp_chinese;
	std::string spanish;
	std::string turkish;

	void initializeAll(const std::string& initText)
	{
		english = initText;
		french = initText;
		german = initText;
		korean = initText;
		russian = initText;
		simp_chinese = initText;
		spanish = initText;
	}

	[[nodiscard]] std::map<std::string, std::string> getAllNames() const
	{
		return std::map<std::string, std::string>{std::pair{"braz_por", braz_por},
			 std::pair{"english", english},
			 std::pair{"french", french},
			 std::pair{"german", german},
			 std::pair{"japanese", japanese},
			 std::pair{"korean", korean},
			 std::pair{"polish", polish},
			 std::pair{"russian", russian},
			 std::pair{"simp_chinese", simp_chinese},
			 std::pair{"spanish", spanish},
			 std::pair{"turkish", turkish}};
	}

} LocBlock;

// Returns substring of str before nth occurrence of match string.
std::string getLeadStr(const std::string& str, const int occurrence = 1, const std::string& match = "_");
// Returns substring of string after nth occurrence of match string.
std::string getTailStr(const std::string& str, const int occurrence = 1, const std::string& match = "_");
// Returns loc without pesky CK3 #EMP STRING#
std::string cleanLocMarkups(const std::string& loc);

class EU5LocalizationLoader
{
  public:
	EU5LocalizationLoader() = default;
	void scrapeLocalizations(const Configuration& theConfiguration, const Mods& mods);
	void scrapeStream(std::istream& theStream, const std::string& language);
	void unravelNestedLocs(LocBlock& block) const;
	void removeEmphasis(LocBlock& block) const;

	[[nodiscard]] std::optional<LocBlock> getLocBlockForKey(const std::string& key) const;
	[[nodiscard]] std::optional<std::string> reverseLookupCultureName(const std::string& localization) const;
	[[nodiscard]] std::set<std::string> reverseLookup(const std::string& localization) const;

  private:
	void scrapeLanguage(const std::string& language, const std::filesystem::path& path);
	std::string selectLanguage(const std::string& language, const LocBlock& block) const;
	void assignSelectLanguage(const std::string& str, const std::string& language, LocBlock& block) const;

	[[nodiscard]] static std::pair<std::string, std::string> determineKeyLocalizationPair(const std::string& text);

	std::map<std::string, LocBlock> localizations;
};
} // namespace EU5

#endif // EU5_LOCALIZATION_LOADER