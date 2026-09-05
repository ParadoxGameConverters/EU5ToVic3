#ifndef POPULATION_MANAGER
#define POPULATION_MANAGER
#include "EU5Pop.h"
#include "Parser.h"
#include <map>
#include <memory>

namespace EU5
{
class PopulationManager: commonItems::parser
{
  public:
	PopulationManager() = default;

	void loadPopulation(std::istream& theStream);

	[[nodiscard]] const auto& getPops() const { return pops; }
	[[nodiscard]] std::shared_ptr<Pop> getPopByID(int thePopID) const;

  private:
	void registerKeys();

	std::map<int, std::shared_ptr<Pop>> pops;
	parser populationDatabaseParser;
};
} // namespace EU5

#endif // POPULATION_MANAGER
