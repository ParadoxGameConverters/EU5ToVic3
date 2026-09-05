#ifndef EU5_PRODUCTION_METHOD_H
#define EU5_PRODUCTION_METHOD_H
#include "Parser.h"
#include <string>
#include <vector>

namespace EU5
{
class ProductionMethod: commonItems::parser
{
  public:
	ProductionMethod() = default;
	explicit ProductionMethod(std::istream& theStream);

	[[nodiscard]] const auto& getProducedGood() const { return producedGood; }
	[[nodiscard]] const auto& getInputGoods() const { return inputGoods; }

  private:
	void registerKeys();

	std::string producedGood;
	std::vector<std::string> inputGoods;
};
} // namespace EU5

#endif // EU5_PRODUCTION_METHOD_H
