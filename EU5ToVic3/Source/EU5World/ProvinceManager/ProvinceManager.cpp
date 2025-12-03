#include "ProvinceManager.h"
#include "Log.h"
#include <ranges>

void EU5::ProvinceManager::registerLocation(int theLocationID, const std::string& theSystemName)
{
	if (seenLocations.contains(theSystemName))
	{
		Log(LogLevel::Error) << "Attempting to register location " << theSystemName << " (" << theLocationID << ")  which is already registered. This is bad.";
		return;
	}

	auto newLocation = std::make_shared<Location>(theLocationID, theSystemName);
	seenLocations.emplace(theSystemName, newLocation);
}

const std::shared_ptr<EU5::Location>& EU5::ProvinceManager::getSeenLocationByID(int theID) const
{
	for (const auto& Location: seenLocations | std::views::values)
	{
		if (Location->getLocationID() == theID)
			return Location;
	}
	return nullptr;
}