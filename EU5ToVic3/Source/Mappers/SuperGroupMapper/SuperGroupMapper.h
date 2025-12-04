#ifndef SUPER_GROUP_MAPPER_H
#define SUPER_GROUP_MAPPER_H
#include "Parser.h"
#include <map>
#include <set>

namespace mappers
{
class SuperGroupMapper: commonItems::parser
{
  public:
	SuperGroupMapper() = default;

	void loadSuperGroups();
	void loadSuperGroups(std::istream& theStream);

	[[nodiscard]] std::optional<std::string> getGroupForContinent(const std::string& continentName) const;

  private:
	void registerKeys();

	std::map<std::string, std::set<std::string>> superGroups; // name (old_world) - continents in it
};
} // namespace mappers

#endif // SUPER_GROUP_MAPPER_H