#ifndef NAMED_COLORS_LOADER_H
#define NAMED_COLORS_LOADER_H
#include "Color.h"
#include "Parser.h"
extern commonItems::Color::Factory laFabricaDeColor;

namespace EU5
{
class NamedColorsLoader: commonItems::parser
{
  public:
	NamedColorsLoader() = default;
	void loadColors(std::istream& theStream);
	void loadColors(const std::filesystem::path& filepath);

  private:
	void registerKeys();
};
} // namespace mappers

#endif // NAMED_COLORS_LOADER_H