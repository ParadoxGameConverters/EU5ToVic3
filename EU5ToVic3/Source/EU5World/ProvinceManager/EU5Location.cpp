#include "EU5Location.h"

EU5::Location::Location(int theLocationID, std::string theLocationName): locID(theLocationID), name(std::move(theLocationName))
{
}