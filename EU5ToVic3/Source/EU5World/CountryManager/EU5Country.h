#ifndef EU5_COUNTRY_H
#define EU5_COUNTRY_H
#include "Parser.h"

namespace EU5
{
class Country: commonItems::parser
{
  public:
	Country() = default;
	Country(int theCountryID, std::istream& theStream);

	// Tag and links
	void setTag(const std::string& theTag) { tag = theTag; }
	[[nodiscard]] const auto& getID() const { return countryID; }
	[[nodiscard]] const auto& getTag() const { return tag; }

	void setRevolutionaryTarget() { revolutionaryTarget = true; }

  private:
	void registerKeys();

	std::string countryName; // This is the localization key used for map name display. SWE or blagodarnoye.
	std::string adjective;	 // this is apparently an optional field. :/
	std::string flag;			 // should tag, AGA94?
	std::string type;			 // location, banking?
	std::string historical;	 // presumably same as tag.
	std::string countryType; // "Real"?? Maybe rebels and so on?
	std::string tag;			 // the actual tag
	int countryID = 0;		 // within the savegame.

	bool revolutionaryTarget = false;
};
} // namespace EU5

#endif // EU5_COUNTRY_H
