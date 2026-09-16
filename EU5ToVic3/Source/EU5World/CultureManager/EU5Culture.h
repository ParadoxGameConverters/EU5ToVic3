#ifndef EU5_CULTURE_H
#define EU5_CULTURE_H
#include "Color.h"
#include "Parser.h"
#include <string>

namespace EU5
{
class Culture: commonItems::parser
{
  public:
	Culture() = default;
	Culture(int theCultureID, std::istream& theStream);

	[[nodiscard]] int getID() const { return cultureID; }
	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getCultureDefinition() const { return cultureDefinition; }
	[[nodiscard]] const auto& getColor() const { return color; }
	[[nodiscard]] const auto& getLanguage() const { return language; }
	[[nodiscard]] double getSize() const { return size; }
	[[nodiscard]] double getCulturalInfluence() const { return culturalInfluence; }
	[[nodiscard]] double getCulturalTradition() const { return culturalTradition; }
	[[nodiscard]] bool getActive() const { return active; }

  private:
	void registerKeys();

	int cultureID = 0;
	std::string name;
	std::string cultureDefinition;
	commonItems::Color color;
	std::string language;
	double size = 0;
	double culturalInfluence = 0;
	double culturalTradition = 0;
	bool active = true;
};
} // namespace EU5

#endif // EU5_CULTURE_H
