#include "V3World.h"
#include "Configuration.h"
#include "EU5World/World.h"
#include "Log.h"
#include "ModLoader/ModLoader.h"


V3::World::World(const Configuration& configuration, const EU5::World& sourceWorld):
	 V3Path(configuration.getVic3Path()),
	 configBlock(configuration.configBlock),
	 datingData(sourceWorld.getDatingData())
{
	Mods overrideMods;
	// We use decentralized world mod to fill out wasteland and out-of-scope clay with decentralized tribes.
	overrideMods.emplace_back("Decentralized World", "configurables/decentralized_world/");
	const auto vanillaFS = commonItems::ModFilesystem(V3Path, {});
	const auto dwFS = commonItems::ModFilesystem(V3Path, overrideMods);
	overrideMods.emplace_back("Blankmod", "blankMod/output/");
	const auto allFS = commonItems::ModFilesystem(V3Path, overrideMods);
	overrideMods.clear();
	overrideMods.emplace_back("Blankmod", "blankMod/output/");
	const auto blankModFS = commonItems::ModFilesystem(V3Path, overrideMods);

	Log(LogLevel::Progress) << "45 %";
	Log(LogLevel::Info) << "* Soaking up the shine *";

	Log(LogLevel::Info) << "*** Hello Vicky 3, creating world. ***";
	Log(LogLevel::Progress) << "46 %";
	// chunks and substate generation

	Log(LogLevel::Progress) << "47 %";
	// initializing countries from eu4 and vanilla

	Log(LogLevel::Progress) << "48 %";
	// assigning clay to countries

	Log(LogLevel::Progress) << "49 %";
	// soaking up vanilla pops

	// inject vanilla substates into map holes.

	Log(LogLevel::Progress) << "50 %";
	// handling demographics

	Log(LogLevel::Progress) << "51 %";
	// generating decentralized countries

	Log(LogLevel::Progress) << "52 %";
	// converting all 3 types of countries - generated decentralized, extinct/extant-VN vanilla-only, and EU4 imports.

	Log(LogLevel::Progress) << "53 %";

	Log(LogLevel::Progress) << "54 %";

	Log(LogLevel::Progress) << "55 %";

	Log(LogLevel::Progress) << "56 %";
	Log(LogLevel::Progress) << "57 %";
	Log(LogLevel::Progress) << "58 %";

	Log(LogLevel::Progress) << "59 %";
	Log(LogLevel::Progress) << "60 %";

	Log(LogLevel::Progress) << "61 %";
	Log(LogLevel::Progress) << "62 %";

	Log(LogLevel::Progress) << "63 %";
	Log(LogLevel::Progress) << "64 %";

	Log(LogLevel::Progress) << "65 %";
	Log(LogLevel::Progress) << "66 %";
	Log(LogLevel::Progress) << "67 %";
	Log(LogLevel::Progress) << "68 %";
	Log(LogLevel::Progress) << "69 %";
	Log(LogLevel::Progress) << "70 %";

	// Place starting buildings for all centralized countries
	Log(LogLevel::Progress) << "71 %";
	Log(LogLevel::Progress) << "72 %";
	Log(LogLevel::Progress) << "73 %";
	Log(LogLevel::Progress) << "74 %";
	Log(LogLevel::Progress) << "75 %";
	Log(LogLevel::Progress) << "76 %";

	Log(LogLevel::Info) << "*** Goodbye, Vicky 3, and godspeed. ***";
}
