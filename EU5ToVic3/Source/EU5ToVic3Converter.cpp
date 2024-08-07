#include "EU5ToVic3Converter.h"
#include "Configuration.h"
#include "EU5World/World.h"
#include "Log.h"
#include "V3World/V3World.h"
#include "outWorld.h"

void convertEU4ToVic3(commonItems::ConverterVersion&& converterVersion)
{
	Log(LogLevel::Progress) << "0 %";
	auto configuration = std::make_shared<Configuration>(converterVersion);
	Log(LogLevel::Info) << "<> Configuration imported.";
	Log(LogLevel::Progress) << "4 %";

	const EU5::World sourceWorld(configuration, converterVersion);
	const V3::World destWorld(*configuration, sourceWorld);
	OUT::exportWorld(*configuration, destWorld, converterVersion);

	Log(LogLevel::Notice) << "* Conversion complete *";
	Log(LogLevel::Progress) << "100 %";
}
