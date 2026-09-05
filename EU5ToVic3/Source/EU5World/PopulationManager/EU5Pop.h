#ifndef EU5_POP_H
#define EU5_POP_H
#include "Parser.h"
#include <map>
#include <string>

namespace EU5
{
class Pop: commonItems::parser
{
  public:
	Pop() = default;
	Pop(int thePopID, std::istream& theStream);

	[[nodiscard]] int getID() const { return popID; }
	[[nodiscard]] const auto& getType() const { return type; }
	[[nodiscard]] const auto& getEstate() const { return estate; }
	[[nodiscard]] int getCultureID() const { return cultureID; }
	[[nodiscard]] const auto& getStatus() const { return status; }
	[[nodiscard]] int getReligionID() const { return religionID; }
	[[nodiscard]] double getSatisfaction() const { return satisfaction; }
	[[nodiscard]] double getSize() const { return size; }
	[[nodiscard]] double getLiteracy() const { return literacy; }

  private:
	void registerKeys();

	int popID = 0;
	std::string type;
	std::string estate; // "burgher_estate"

	int cultureID = 0;
	std::string status; // "Accepted", "Primary", etc
	int religionID = 0;
	double satisfaction = 0;
	double size = 0; // unit is thousands I think
	double literacy = 0;
};
} // namespace EU5

#endif // EU5_POP_H