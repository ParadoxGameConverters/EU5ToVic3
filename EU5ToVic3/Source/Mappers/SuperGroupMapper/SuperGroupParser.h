#ifndef SUPERGROUP_PARSER
#define SUPERGROUP_PARSER
#include "Parser.h"
#include <set>

namespace mappers
{
class SuperGroupParser: commonItems::parser
{
  public:
	explicit SuperGroupParser(std::istream& theStream);

	[[nodiscard]] const auto& getContinentNames() const { return continentNames; }

  private:
	void registerKeys();

	std::set<std::string> continentNames;
};
} // namespace mappers

#endif // SUPERGROUP_PARSER