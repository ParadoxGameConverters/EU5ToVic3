#ifndef CULTURE_MANAGER_H
#define CULTURE_MANAGER_H
#include "EU5Culture.h"
#include "Parser.h"
#include <map>
#include <memory>

namespace EU5
{
class CultureManager: commonItems::parser
{
  public:
	CultureManager() = default;

	void loadCultures(std::istream& theStream);

	[[nodiscard]] const auto& getCultures() const { return cultures; }
	[[nodiscard]] std::shared_ptr<Culture> getCultureByID(int theCultureID) const;

  private:
	void registerKeys();

	std::map<int, std::shared_ptr<Culture>> cultures;
	parser cultureDatabaseParser;
};
} // namespace EU5

#endif // CULTURE_MANAGER_H
