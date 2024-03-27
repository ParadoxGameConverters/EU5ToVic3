#ifndef EU5_WORLD_H
#define EU5_WORLD_H
#include "Configuration.h"
#include "ConverterVersion.h"
#include "DatingData.h"
#include "GameVersion.h"
#include "ModLoader/ModLoader.h"
#include "Parser.h"

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

	struct saveData
	{
		bool parsedMeta = false;
		std::string metadata;
		std::string gamestate;
		std::string path;
	} saveGame;

	std::string EU5Path;
	Mods mods;
	commonItems::ModFilesystem modFS;

	DatingData datingData;

	GameVersion version;
};
} // namespace EU5

#endif // EU5_WORLD_H
