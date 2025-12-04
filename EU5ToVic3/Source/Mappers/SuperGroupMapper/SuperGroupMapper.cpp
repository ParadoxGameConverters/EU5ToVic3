#include "SuperGroupMapper.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "SuperGroupParser.h"
#include <filesystem>

void mappers::SuperGroupMapper::loadSuperGroups()
{
	Log(LogLevel::Info) << "-> Parsing starting supergroups.";
	registerKeys();
	if (commonItems::DoesFileExist("configurables/world_supergroups.txt"))
		parseFile("configurables/world_supergroups.txt");
	else if (commonItems::DoesFileExist("TestFiles/configurables/world_supergroups.txt"))
		parseFile("TestFiles/configurables/world_supergroups.txt");
	else
		throw std::runtime_error("Where is configurables/world_supergroups.txt?");
	clearRegisteredKeywords();
}

void mappers::SuperGroupMapper::loadSuperGroups(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::SuperGroupMapper::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& superGroupName, std::istream& theStream) {
		Log(LogLevel::Debug) << "sname " << superGroupName;
		const SuperGroupParser newContinentsBlock(theStream);
		superGroups.emplace(superGroupName, newContinentsBlock.getContinentNames());
	});
}

std::optional<std::string> mappers::SuperGroupMapper::getGroupForContinent(const std::string& continentName) const
{
	for (const auto& [superGroupName, superGroup]: superGroups)
		if (superGroup.contains(continentName))
			return superGroupName;
	return std::nullopt;
}
