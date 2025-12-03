#include "Province.h"
#include "CommonRegexes.h"
#include "Log.h"
#include "ParserHelpers.h"

EU5::Province::Province(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void EU5::Province::registerKeys()
{
	registerKeyword("color", commonItems::ignoreItem);
	registerRegex(commonItems::integerRegex, [this](const std::string& number, std::istream& theStream) {
		// This is a peculiar file format where we pull free-floating numbers from thin air.
		// The regex itself will consume the stream.
		try
		{
			provinces.emplace(std::stoi(number));
		}
		catch (std::exception& e)
		{
			Log(LogLevel::Error) << "Broken province inside area.txt?! " << e.what();
			throw std::runtime_error(e.what());
		}
	});
}

bool EU5::Province::provinceContainsLocation(const std::string& location) const
{
	return locations.contains(location);
}
