#include "BuildingTypeLoader.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

void EU5::BuildingTypeLoader::loadBuildingTypes(const commonItems::ModFilesystem& modFS)
{
	registerKeys();
	for (const auto& file: modFS.GetAllFilesInFolder("in_game/common/building_types"))
	{
		if (file.extension() != ".txt")
			continue;
		parseFile(file);
	}
	clearRegisteredKeywords();
}

void EU5::BuildingTypeLoader::loadBuildingTypes(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::BuildingTypeLoader::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& buildingType, std::istream& theStream) {
		parser typeParser;
		typeParser.registerKeyword("pop_type", [this, &buildingType](std::istream& popTypeStream) {
			popTypes.emplace(buildingType, commonItems::getString(popTypeStream));
		});
		typeParser.registerKeyword("unique_production_methods", [this](std::istream& methodsStream) {
			parser methodsParser;
			methodsParser.registerRegex(commonItems::catchallRegex, [this](const std::string& methodKey, std::istream& methodStream) {
				productionMethods.emplace(methodKey, ProductionMethod(methodStream));
			});
			methodsParser.parseStream(methodsStream);
		});
		typeParser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
		typeParser.parseStream(theStream);
	});
}

std::optional<std::string> EU5::BuildingTypeLoader::getPopType(const std::string& buildingType) const
{
	if (const auto& itr = popTypes.find(buildingType); itr != popTypes.end())
		return itr->second;
	return std::nullopt;
}

std::optional<EU5::ProductionMethod> EU5::BuildingTypeLoader::getProductionMethod(const std::string& productionMethodKey) const
{
	if (const auto& itr = productionMethods.find(productionMethodKey); itr != productionMethods.end())
		return itr->second;
	return std::nullopt;
}
