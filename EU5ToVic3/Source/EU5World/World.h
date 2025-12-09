#ifndef EU5_WORLD_H
#define EU5_WORLD_H
#include "Configuration.h"
#include "ConverterVersion.h"
#include "CountryManager/EU5CountryManager.h"
#include "DatingData.h"
#include "GameVersion.h"
#include "Mappers/NamedColors/NamedColorsMapper.h"
#include "ModLoader/ModFilesystem.h"
#include "ModLoader/ModLoader.h"
#include "Parser.h"
#include "ProvinceManager/ProvinceManager.h"

namespace EU5
{
class World: commonItems::parser
{
  public:
	World(const std::shared_ptr<Configuration>& theConfiguration, const commonItems::ConverterVersion& converterVersion);

	// V3World inputs

	[[nodiscard]] const auto& getDatingData() const { return datingData; }
	[[nodiscard]] const auto& getEU5ModFS() const { return modFS; }

  private:
	void registerKeys(const std::shared_ptr<Configuration>& theConfiguration, const commonItems::ConverterVersion& converterVersion);
	void verifySave();
	void primeLaFabricaDeColor();

	parser metaPreParser;
	parser metaParser;
	parser compatibilityParser;

	struct saveData
	{
		bool parsedMeta = false;
		std::string metadata;
		std::string gamestate;
		std::filesystem::path path;
	} saveGame;

	std::filesystem::path EU5Path;
	Mods mods;
	commonItems::ModFilesystem modFS;

	DatingData datingData;
	GameVersion version;

	ProvinceManager provinceManager;
	mappers::NamedColorsMapper namedColors;
	CountryManager countryManager;
};
} // namespace EU5

#endif // EU5_WORLD_H
