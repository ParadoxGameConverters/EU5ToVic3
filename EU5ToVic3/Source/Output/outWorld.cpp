#include "outWorld.h"
#include "CommonFunctions.h"
#include "OSCompatibilityLayer.h"
#include "outMetadataFile/outMetadataFile.h"
#include <fstream>

void OUT::exportWorld(const Configuration& configuration, const V3::World& world, const commonItems::ConverterVersion& converterVersion)
{
	const auto& outputName = configuration.getOutputName();

	Log(LogLevel::Info) << "---> Le Dump <---";
	std::filesystem::create_directories("output");
	Log(LogLevel::Progress) << "80 %";

	// Delete old conversion
	if (commonItems::DoesFolderExist("output" / outputName))
	{
		Log(LogLevel::Info) << "<< Deleting existing mod folder.";
		std::filesystem::remove_all("output" / outputName);
	}
	Log(LogLevel::Progress) << "81 %";

	Log(LogLevel::Info) << "<< Copying Mod Template from blankMod/output to output/" << outputName.string();
	std::filesystem::copy("blankMod/output",
		 "output" / outputName,
		 std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
	Log(LogLevel::Progress) << "83 %";

	Log(LogLevel::Info) << "<- Crafting .metadata File";
	exportMetadataFile(outputName);
	Log(LogLevel::Progress) << "84 %";

	// Record converter version
	Log(LogLevel::Info) << "<- Writing version";
	exportVersion(outputName, converterVersion);
	Log(LogLevel::Progress) << "85 %";

	// Update bookmark starting dates
	Log(LogLevel::Info) << "<- Updating bookmarks";
	exportBookmark(outputName, configuration, world.getDatingData());
	Log(LogLevel::Progress) << "86 %";

	Log(LogLevel::Info) << "<- Dumping common/history/states";
	Log(LogLevel::Progress) << "87 %";

	Log(LogLevel::Info) << "<- Copying Custom Flags";
	copyCustomFlags(outputName);
	Log(LogLevel::Progress) << "88 %";
	Log(LogLevel::Info) << "<- Writing Custom CoAs";
	Log(LogLevel::Progress) << "89 %";
	Log(LogLevel::Info) << "<- Writing Flag Definitions";
	Log(LogLevel::Progress) << "90 %";

	Log(LogLevel::Info) << "<- Writing Localizations";
	Log(LogLevel::Progress) << "91 %";

	Log(LogLevel::Info) << "<- Writing Countries";
	Log(LogLevel::Progress) << "92 %";

	Log(LogLevel::Info) << "<- Writing Diplomacy";
	Log(LogLevel::Progress) << "93 %";

	// Log(LogLevel::Info) << "<- Writing Armed and Unarmed Conflicts";

	Log(LogLevel::Info) << "<- Writing AI Strategies";
	Log(LogLevel::Progress) << "94 %";

	Log(LogLevel::Info) << "<- Writing Culture Definitions";
	Log(LogLevel::Progress) << "95 %";

	Log(LogLevel::Info) << "<- Writing Religion Definitions";
	Log(LogLevel::Progress) << "96 %";

	Log(LogLevel::Info) << "<- Writing Pops";
	Log(LogLevel::Progress) << "97 %";

	Log(LogLevel::Info) << "<- Writing Characters";
	Log(LogLevel::Progress) << "98 %";

	Log(LogLevel::Info) << "<- Writing Buildings";
	Log(LogLevel::Progress) << "99 %";
}

void OUT::exportVersion(const std::filesystem::path& outputName, const commonItems::ConverterVersion& converterVersion)
{
	std::ofstream output("output" / outputName / "eu4tovic3_version.txt");
	if (!output.is_open())
		throw std::runtime_error("Error writing version file! Is the output folder writable?");
	output << converterVersion;
	output.close();
}

void OUT::exportBookmark(const std::filesystem::path& outputName, const Configuration& configuration, const DatingData& datingData)
{
	if (configuration.configBlock.startDate == Configuration::STARTDATE::Vanilla)
		return;
	std::ofstream output("output" / outputName / "common/defines/99_converter_defines.txt");
	if (!output.is_open())
		throw std::runtime_error("Error writing defines file! Is the output folder writable?");
	output << commonItems::utf8BOM << "NGame = { START_DATE = \"" << datingData.lastEU4Date << "\" }\n";
	output.close();
}

void OUT::copyCustomFlags(const std::filesystem::path& outputName)
{
	auto counter = 0;
	if (!commonItems::DoesFolderExist(std::filesystem::path("flags.tmp")))
	{
		Log(LogLevel::Warning) << "Flag folder flags.tmp not found!";
		return;
	}
	for (const auto& filename: commonItems::GetAllFilesInFolder(std::filesystem::path("flags.tmp")))
	{
		std::filesystem::copy_file("flags.tmp" / filename,
			 "output" / outputName / "/gfx/coat_of_arms/textured_emblems/custom_export/" / filename,
			 std::filesystem::copy_options::overwrite_existing);
	}
	Log(LogLevel::Info) << "<< " << counter << " flags copied.";
}
