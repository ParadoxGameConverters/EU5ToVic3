#ifndef EU5_MODNAMES_H
#define EU5_MODNAMES_H
#include "ConvenientParser.h"

namespace EU5
{
class ModNames: commonItems::convenientParser
{
  public:
	explicit ModNames(std::istream& theStream);

	[[nodiscard]] const auto& getName() const { return name; }
	[[nodiscard]] const auto& getPath() const { return path; }

  private:
	void registerKeys();

	std::string name;
	std::string path;
};
} // namespace EU5

#endif // EU5_MODNAMES_H