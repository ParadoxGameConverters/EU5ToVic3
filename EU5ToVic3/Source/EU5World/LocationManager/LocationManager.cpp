#include "LocationManager.h"
#include "Log.h"
#include <ranges>

void EU5::LocationManager::loadLocations(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::LocationManager::registerKeys()
{
	registerKeyword("locations", [this](const std::string& unused, std::istream& theStream) {
		parseStream(theStream); // Double-tapping locations keyword.
	});
	registerRegex(commonItems::integerRegex, [this](const std::string& numberString, std::istream& theStream) {
		// Step 1. Find what location we're talking about.
		const auto theLocation = getSeenLocationByID(std::stoi(numberString));
		if (!theLocation)
		{
			Log(LogLevel::Error) << "Attempting to load location data for (" << numberString << ") which is NOT registered. THIS IS CORRUPTION-LEVEL BAD.";
			return;
		}
		theLocation->parseData(theStream);
		locations.emplace(theLocation->getName(), theLocation);
		// There is no Step 2.
	});
	registerRegex(commonItems::catchallRegex, commonItems::ignoreItem);
}

void EU5::LocationManager::registerLocation(int theLocationID, const std::string& locationName)
{
	if (seenLocations.contains(locationName))
	{
		Log(LogLevel::Error) << "Attempting to register location " << locationName << " (" << theLocationID << ")  which is already registered. This is bad.";
		return;
	}

	auto newLocation = std::make_shared<Location>(theLocationID, locationName);
	seenLocations.emplace(locationName, newLocation);
}

std::shared_ptr<EU5::Location> EU5::LocationManager::getSeenLocationByID(int theID) const
{
	for (const auto& Location: seenLocations | std::views::values)
	{
		if (Location->getID() == theID)
			return Location;
	}

	Log(LogLevel::Error) << "Trying to access seenLocations by ID " << std::to_string(theID) << " which does not exists! Fire! Bad!";
	return nullptr;
}