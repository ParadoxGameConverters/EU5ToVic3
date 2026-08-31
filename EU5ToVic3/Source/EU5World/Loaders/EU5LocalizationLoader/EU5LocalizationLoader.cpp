#include "EU5LocalizationLoader.h"
#include "Configuration/Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <algorithm>
#include <array>
#include <fstream>
#include <ranges>
#include <set>

namespace
{
constexpr std::array<std::pair<const char*, std::string EU5::LocBlock::*>, 11> languageFields{{
	 {"braz_por", &EU5::LocBlock::braz_por},
	 {"english", &EU5::LocBlock::english},
	 {"french", &EU5::LocBlock::french},
	 {"german", &EU5::LocBlock::german},
	 {"japanese", &EU5::LocBlock::japanese},
	 {"korean", &EU5::LocBlock::korean},
	 {"polish", &EU5::LocBlock::polish},
	 {"russian", &EU5::LocBlock::russian},
	 {"simp_chinese", &EU5::LocBlock::simp_chinese},
	 {"spanish", &EU5::LocBlock::spanish},
	 {"turkish", &EU5::LocBlock::turkish},
}};
} // namespace

void EU5::EU5LocalizationLoader::scrapeLocalizations(const Configuration& theConfiguration, const Mods& mods)
{
	for (const auto& field: languageFields)
		scrapeLanguage(std::string(field.first), theConfiguration.getEU5Path() / "main_menu/localization");

	for (const auto& mod: mods)
	{
		if (commonItems::DoesFolderExist(mod.path / "main_menu/localization"))
		{
			Log(LogLevel::Info) << "\t>> Found some words in: [" << mod.name + "]";
			for (const auto& field: languageFields)
				scrapeLanguage(std::string(field.first), mod.path / "main_menu/localization");
			for (const auto& field: languageFields)
				scrapeLanguage(std::string(field.first), mod.path / "main_menu/localization/replace");
		}
	}

	Log(LogLevel::Info) << ">> " << localizations.size() << " words read.";
}

void EU5::EU5LocalizationLoader::scrapeLanguage(const std::string& language, const std::filesystem::path& path)
{
	if (!commonItems::DoesFolderExist(path / language))
		return;
	const auto fileNames = commonItems::GetAllFilesInFolderRecursive(path / language);
	for (const auto& file: fileNames)
	{
		std::ifstream fileStream(path / language / file);
		if (fileStream.is_open())
			scrapeStream(fileStream, language);
		fileStream.close();
	}
}

void EU5::EU5LocalizationLoader::unravelNestedLocs(LocBlock& block) const
{
	// Support Korean/Chinese/Russian?
	for (const auto& field: languageFields | std::views::values)
	{
		const auto& loc = block.*field;
		if (loc.find('$') != std::string::npos) // TODO: handle escaped \$
		{
			const auto& keyStr = getLeadStr(loc, 2, "$"); // Chop off tail after nested key
			auto nestedKey = getTailStr(keyStr, 1, "$");	 // Chop off head before nested key
			if (nestedKey.find('|') != std::string::npos)
			{
				nestedKey = getLeadStr(nestedKey, 1, "|"); // chop off localization specifics within the nested key. We can't handle them anyway.
			}
			if (const auto& newblock = getLocBlockForKey(nestedKey); newblock)
			{
				const auto& fstr = getLeadStr(loc, 1, "$");
				const auto& bstr = getTailStr(loc, 2, "$");
				block.*field = commonItems::remQuotes(fstr + newblock.value().*field + bstr);
			}
			else
			{
				Log(LogLevel::Warning) << "Could not find locblock for nested loc: " + nestedKey;
				return;
			}
			unravelNestedLocs(block);
		}
	}
}

void EU5::EU5LocalizationLoader::removeEmphasis(LocBlock& block) const
{
	for (const auto& field: languageFields | std::views::values)
		block.*field = commonItems::remQuotes(cleanLocMarkups(block.*field));
}

void EU5::EU5LocalizationLoader::scrapeStream(std::istream& theStream, const std::string& language)
{
	std::string line;
	std::getline(theStream, line); // Dropping first line and BOM with it.

	const auto fieldItr = std::ranges::find_if(languageFields, [&language](const auto& entry) {
		return entry.first == language;
	});
	if (fieldItr == languageFields.end())
		return; // unsupported language, nothing to assign.
	const auto field = fieldItr->second;

	while (!theStream.eof())
	{
		std::getline(theStream, line);
		const auto [key, value] = determineKeyLocalizationPair(line);
		if (!key.empty() && !value.empty())
		{
			if (localizations.contains(key))
			{
				localizations[key].*field = value;
			}
			else
			{
				LocBlock newBlock;
				newBlock.*field = value;
				localizations.insert(std::pair(key, newBlock));
			}
		}
	}
}

std::pair<std::string, std::string> EU5::EU5LocalizationLoader::determineKeyLocalizationPair(const std::string& text)
{
	if (text.size() > 2 && (text[0] == '#' || text[1] == '#'))
		return std::pair<std::string, std::string>{};

	const auto keyBeginPos = text.find_first_not_of(' ');
	if (keyBeginPos == std::string::npos)
		return std::pair<std::string, std::string>{};

	const auto keyEndPos = text.find_first_of(':', keyBeginPos + 1);
	const auto quotePos = text.find_first_of('"', keyEndPos);
	if (quotePos == std::string::npos)
		return std::pair<std::string, std::string>{};

	const auto localizationBeginPos = quotePos + 1;
	const auto localizationEndPos = text.find_last_of('"', text.size());
	return std::make_pair(text.substr(keyBeginPos, keyEndPos - keyBeginPos), text.substr(localizationBeginPos, localizationEndPos - localizationBeginPos));
}

std::optional<EU5::LocBlock> EU5::EU5LocalizationLoader::getLocBlockForKey(const std::string& key) const
{
	if (key.empty())
		return std::nullopt;
	const auto& keyItr = localizations.find(key);
	if (keyItr == localizations.end())
		return std::nullopt;

	// English is absolute base, we bail if it's missing. Can't fill in the rest without it.
	// (Quite a few locs have only russian entries and no other languages.)
	if (keyItr->second.english.empty())
		return std::nullopt;

	auto newBlock = keyItr->second;
	for (const auto& field: languageFields | std::views::values)
		if ((newBlock.*field).empty())
			newBlock.*field = newBlock.english;
	return newBlock;
}

std::set<std::string> EU5::EU5LocalizationLoader::reverseLookup(const std::string& localization) const
{
	// This is a general lookup for a loc key.
	std::set<std::string> toReturn;

	for (const auto& [locName, locBlock]: localizations)
		for (const auto& field: languageFields | std::views::values)
			if (locBlock.*field == localization)
			{
				toReturn.emplace(locName);
				break;
			}

	return toReturn;
}

std::string EU5::getLeadStr(const std::string& str, const int occurrence, const std::string& match)
{
	if (const auto& i = str.find(match); i != std::string::npos)
		if (occurrence == 1)
			return str.substr(0, i);
		else
			return str.substr(0, i) + match + getLeadStr(str.substr(i + match.length()), occurrence - 1, match);
	else
		return str;
}
std::string EU5::getTailStr(const std::string& str, const int occurrence, const std::string& match)
{
	if (const auto& i = str.find(match); i != std::string::npos)
		if (occurrence == 1)
		{
			auto tail = str.substr(i + match.length());
			if (tail.starts_with("!"))
				tail = tail.substr(1, tail.length());
			return tail;
		}
		else
			return getTailStr(str.substr(i + match.length()), occurrence - 1, match);
	else
		return str;
}
std::string EU5::cleanLocMarkups(const std::string& loc)
{
	if (loc.find('#') == std::string::npos)
		return loc;

	// Locmarks come in two styles: #SOMETHING with a whitespace behind it, and a #! with no whitespace trailing it.
	// We iterate over the entire string and just rip these out.

	std::string workingLoc = loc;
	auto pos = workingLoc.find('#');

	while (pos != std::string::npos)
	{
		// find first #
		const auto& head = getLeadStr(workingLoc, 1, "#");

		// find how long it is.
		if (pos <= workingLoc.size() - 2 && workingLoc.at(pos + 1) == '!')
		{
			// Delete #!
			workingLoc = head + workingLoc.substr(pos + 2, workingLoc.size());
		}
		else if (pos <= workingLoc.size() - 2 && workingLoc.at(pos + 1) == ' ')
		{
			// Delete freeform #
			workingLoc = head + workingLoc.substr(pos + 1, workingLoc.size());
		}
		else if (pos == workingLoc.size() - 1)
		{
			// Delete # at endline
			workingLoc = head;
		}
		else
		{
			// find the whitespace ending the mark.
			auto endpos = workingLoc.find(' ', pos + 1);
			if (endpos == std::string::npos)
				return loc; // bail
			workingLoc = head + workingLoc.substr(endpos + 1, workingLoc.size());
		}
		pos = workingLoc.find('#');
	}

	return std::string(workingLoc);
}
