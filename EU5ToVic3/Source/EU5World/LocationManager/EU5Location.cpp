#include "EU5Location.h"

EU5::Location::Location(int theLocationID, std::string theLocationName): locID(theLocationID), name(std::move(theLocationName))
{
}

void EU5::Location::parseData(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::Location::registerKeys()
{
	registerKeyword("owner", [this](std::istream& theStream) {
		ownerID = commonItems::getInt(theStream);
	});
	registerKeyword("controller", [this](std::istream& theStream) {
		controllerID = commonItems::getInt(theStream);
	});
	registerKeyword("previous_owner", [this](std::istream& theStream) {
		previousOwnerID = commonItems::getInt(theStream);
	});
	registerKeyword("previous_controller", [this](std::istream& theStream) {
		previousControllerID = commonItems::getInt(theStream);
	});
	registerKeyword("development", [this](std::istream& theStream) {
		development = commonItems::getDouble(theStream);
	});
	registerKeyword("prosperity", [this](std::istream& theStream) {
		prosperity = commonItems::getDouble(theStream);
	});
	registerKeyword("language", [this](std::istream& theStream) {
		language = commonItems::getString(theStream);
	});
	registerKeyword("dialect", [this](std::istream& theStream) {
		dialect = commonItems::getString(theStream);
	});
	registerKeyword("rank", [this](std::istream& theStream) {
		rank = commonItems::getString(theStream);
	});
	registerKeyword("original_rank", [this](std::istream& theStream) {
		originalRank = commonItems::getString(theStream);
	});
	registerKeyword("raw_material", [this](std::istream& theStream) {
		rawMaterial = commonItems::getString(theStream);
	});
	registerKeyword("cores", [this](std::istream& theStream) {
		const auto& cores = commonItems::getInts(theStream);
		countriesWithCore.insert(cores.begin(), cores.end());
	});

	registerKeyword("religion", [this](std::istream& theStream) {
		religionID = commonItems::getInt(theStream);
	});
	registerKeyword("culture", [this](std::istream& theStream) {
		cultureID = commonItems::getInt(theStream);
	});
	registerKeyword("secondary_culture", [this](std::istream& theStream) {
		secondaryCultureID = commonItems::getInt(theStream);
	});
	registerKeyword("cultural_unity", [this](std::istream& theStream) {
		culturalUnity = commonItems::getDouble(theStream);
	});

	registerKeyword("last_owner_change", [this](std::istream& theStream) {
		lastOwnerChange = date(commonItems::getString(theStream));
	});
	registerKeyword("last_controller_change", [this](std::istream& theStream) {
		lastControllerChange = date(commonItems::getString(theStream));
	});

	registerKeyword("max_raw_material_workers", [this](std::istream& theStream) {
		maxRawMaterialWorkers = commonItems::getInt(theStream);
	});
	registerKeyword("control", [this](std::istream& theStream) {
		control = commonItems::getDouble(theStream);
	});
	registerKeyword("cap_control", [this](std::istream& theStream) {
		capControl = commonItems::getString(theStream) == "yes";
	});
	registerKeyword("road_to_capital", [this](std::istream& theStream) {
		roadToCapital = commonItems::getString(theStream) == "yes";
	});
	registerKeyword("proximity", [this](std::istream& theStream) {
		proximity = commonItems::getDouble(theStream);
	});
	registerKeyword("local_proximity_propagation", [this](std::istream& theStream) {
		localProximityPropagation = commonItems::getDouble(theStream);
	});
	registerKeyword("value_flow", [this](std::istream& theStream) {
		valueFlow = commonItems::getDouble(theStream);
	});

	registerKeyword("province", [this](std::istream& theStream) {
		provinceID = commonItems::getInt(theStream);
	});
	registerKeyword("tax", [this](std::istream& theStream) {
		tax = commonItems::getDouble(theStream);
	});
	registerKeyword("possible_tax", [this](std::istream& theStream) {
		possibleTax = commonItems::getDouble(theStream);
	});
	registerKeyword("estate_tax", [this](std::istream& theStream) {
		for (const auto& [estate, value]: commonItems::assignments(theStream).getAssignments())
			estateTax[estate].tax = commonItems::stringToDouble(value);
	});
	registerKeyword("estate_possible_tax", [this](std::istream& theStream) {
		for (const auto& [estate, value]: commonItems::assignments(theStream).getAssignments())
			estateTax[estate].possibleTax = commonItems::stringToDouble(value);
	});
	registerKeyword("institutions", [this](std::istream& theStream) {
		for (const auto& [institution, spread]: commonItems::assignments(theStream).getAssignments())
			institutions.emplace(institution, commonItems::stringToDouble(spread));
	});

	registerKeyword("units", [this](std::istream& theStream) {
		unitIDs = commonItems::getInts(theStream);
	});
	registerKeyword("port", [this](std::istream& theStream) {
		portIDs = commonItems::getInts(theStream);
	});
	//
	registerKeyword("population", [this](std::istream& theStream) {
		commonItems::parser parser;
		parser.registerKeyword("pops", [this](std::istream& popsStream) {
			popIDs = commonItems::getInts(popsStream);
		});
		parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
		parser.parseStream(theStream);
	});

	registerKeyword("garrison", [this](std::istream& theStream) {
		garrison = commonItems::getDouble(theStream);
	});
	registerKeyword("counters", [this](std::istream& theStream) {
		for (const auto& [counter, value]: commonItems::assignments(theStream).getAssignments())
			counters.emplace(counter, commonItems::stringToInteger<int>(value));
	});

	registerKeyword("market", [this](std::istream& theStream) {
		marketID = commonItems::getInt(theStream);
	});
	registerKeyword("second_best_market", [this](std::istream& theStream) {
		secondBestMarketID = commonItems::getInt(theStream);
	});
	registerKeyword("market_parent", [this](std::istream& theStream) {
		marketParentID = commonItems::getInt(theStream);
	});
	registerKeyword("market_access", [this](std::istream& theStream) {
		marketAccess = commonItems::getDouble(theStream);
	});
	registerKeyword("market_attraction", [this](std::istream& theStream) {
		marketAttraction = commonItems::getDouble(theStream);
	});
	registerKeyword("second_best_market_access", [this](std::istream& theStream) {
		secondBestMarketAccess = commonItems::getDouble(theStream);
	});

	// wanted to organize with country as key in map rather than a list
	// not even sure how many we can see here, max i've seen is 2 so far
	registerKeyword("integration_data", [this](std::istream& theStream) {
		for (const auto& blob: commonItems::blobList(theStream).getBlobs())
		{
			std::istringstream blobStream(blob);
			int owner = 0;
			Integration integration;
			commonItems::parser parser;
			parser.registerKeyword("integration", [&integration](std::istream& itemStream) {
				integration.type = commonItems::getString(itemStream);
			});
			parser.registerKeyword("integration_progress", [&integration](std::istream& itemStream) {
				integration.progress = commonItems::getDouble(itemStream);
			});
			parser.registerKeyword("integration_owner", [&owner](std::istream& itemStream) {
				owner = commonItems::getInt(itemStream);
			});
			parser.registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
			parser.parseStream(blobStream);
			integrationData.emplace(owner, integration);
		}
	});

	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}
