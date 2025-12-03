#include "EU5Location.h"

EU5::Location::Location(int theLocationID, std::string theSystemName): locID(theLocationID), systemName(std::move(theSystemName))
{
}