#include "EU5LocalizationLoader.h"
#include "Configuration/Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <fstream>
#include <set>

void EU5::EU5LocalizationLoader::scrapeLocalizations(const Configuration& theConfiguration, const Mods& mods)
{
	scrapeLanguage("braz_por", theConfiguration.getEU5Path() / "main_menu/localization");
	scrapeLanguage("english", theConfiguration.getEU5Path() / "main_menu/localization");
	scrapeLanguage("french", theConfiguration.getEU5Path() / "main_menu/localization");
	scrapeLanguage("german", theConfiguration.getEU5Path() / "main_menu/localization");
	scrapeLanguage("japanese", theConfiguration.getEU5Path() / "main_menu/localization");
	scrapeLanguage("korean", theConfiguration.getEU5Path() / "main_menu/localization");
	scrapeLanguage("polish", theConfiguration.getEU5Path() / "main_menu/localization");
	scrapeLanguage("russian", theConfiguration.getEU5Path() / "main_menu/localization");
	scrapeLanguage("simp_chinese", theConfiguration.getEU5Path() / "main_menu/localization");
	scrapeLanguage("spanish", theConfiguration.getEU5Path() / "main_menu/localization");
	scrapeLanguage("turkish", theConfiguration.getEU5Path() / "main_menu/localization");

	for (const auto& mod: mods)
	{
		if (commonItems::DoesFolderExist(mod.path / "main_menu/localization"))
		{
			Log(LogLevel::Info) << "\t>> Found some words in: [" << mod.name + "]";
			scrapeLanguage("braz_por", mod.path / "main_menu/localization");
			scrapeLanguage("english", mod.path / "main_menu/localization");
			scrapeLanguage("french", mod.path / "main_menu/localization");
			scrapeLanguage("german", mod.path / "main_menu/localization");
			scrapeLanguage("japanese", mod.path / "main_menu/localization");
			scrapeLanguage("korean", mod.path / "main_menu/localization");
			scrapeLanguage("polish", mod.path / "main_menu/localization");
			scrapeLanguage("russian", mod.path / "main_menu/localization");
			scrapeLanguage("simp_chinese", mod.path / "main_menu/localization");
			scrapeLanguage("spanish", mod.path / "main_menu/localization");
			scrapeLanguage("turkish", mod.path / "main_menu/localization");

			scrapeLanguage("braz_por", mod.path / "main_menu/localization/replace");
			scrapeLanguage("english", mod.path / "main_menu/localization/replace");
			scrapeLanguage("french", mod.path / "main_menu/localization/replace");
			scrapeLanguage("german", mod.path / "main_menu/localization/replace");
			scrapeLanguage("japanese", mod.path / "main_menu/localization/replace");
			scrapeLanguage("korean", mod.path / "main_menu/localization/replace");
			scrapeLanguage("polish", mod.path / "main_menu/localization/replace");
			scrapeLanguage("russian", mod.path / "main_menu/localization/replace");
			scrapeLanguage("simp_chinese", mod.path / "main_menu/localization/replace");
			scrapeLanguage("spanish", mod.path / "main_menu/localization/replace");
			scrapeLanguage("turkish", mod.path / "main_menu/localization/replace");
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
	for (const auto& lang:
		 std::vector<std::string>{"braz_por", "english", "french", "german", "japanese", "korean", "polish", "russian", "simp_chinese", "spanish", "turkish"})
	{
		const auto& loc = selectLanguage(lang, block);
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
				assignSelectLanguage(fstr + selectLanguage(lang, newblock.value()) + bstr, lang, block);
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
	for (const auto& lang:
		 std::vector<std::string>{"braz_por", "english", "french", "german", "japanese", "korean", "polish", "russian", "simp_chinese", "spanish", "turkish"})
	{
		const auto& loc = selectLanguage(lang, block);
		assignSelectLanguage(cleanLocMarkups(loc), lang, block);
	}
}

std::string EU5::EU5LocalizationLoader::selectLanguage(const std::string& language, const LocBlock& block) const
{
	if (language == "braz_por")
		return block.braz_por;
	if (language == "french")
		return block.french;
	if (language == "german")
		return block.german;
	if (language == "japanese")
		return block.japanese;
	if (language == "korean")
		return block.korean;
	if (language == "polish")
		return block.polish;
	if (language == "russian")
		return block.russian;
	if (language == "simp_chinese")
		return block.simp_chinese;
	if (language == "spanish")
		return block.spanish;
	if (language == "turkish")
		return block.turkish;
	return block.english;
}

void EU5::EU5LocalizationLoader::assignSelectLanguage(const std::string& str, const std::string& language, LocBlock& block) const
{
	if (language == "braz_por")
		block.braz_por = commonItems::remQuotes(str);
	else if (language == "english")
		block.english = commonItems::remQuotes(str);
	else if (language == "french")
		block.french = commonItems::remQuotes(str);
	else if (language == "spanish")
		block.spanish = commonItems::remQuotes(str);
	else if (language == "german")
		block.german = commonItems::remQuotes(str);
	else if (language == "japanese")
		block.japanese = commonItems::remQuotes(str);
	else if (language == "korean")
		block.korean = commonItems::remQuotes(str);
	else if (language == "polish")
		block.polish = commonItems::remQuotes(str);
	else if (language == "russian")
		block.russian = commonItems::remQuotes(str);
	else if (language == "simp_chinese")
		block.simp_chinese = commonItems::remQuotes(str);
	else if (language == "turkish")
		block.turkish = commonItems::remQuotes(str);
	else
		throw std::invalid_argument(language + " is not currently supported or has a typo.");
}

void EU5::EU5LocalizationLoader::scrapeStream(std::istream& theStream, const std::string& language)
{
	std::string line;
	std::getline(theStream, line); // Dropping first line and BOM with it.

	while (!theStream.eof())
	{
		std::getline(theStream, line);
		const auto [key, value] = determineKeyLocalizationPair(line);
		if (!key.empty() && !value.empty())
		{
			if (localizations.contains(key))
			{
				if (language == "braz_por")
					localizations[key].braz_por = value;
				if (language == "english")
					localizations[key].english = value;
				if (language == "french")
					localizations[key].french = value;
				if (language == "german")
					localizations[key].german = value;
				if (language == "japanese")
					localizations[key].japanese = value;
				if (language == "korean")
					localizations[key].korean = value;
				if (language == "polish")
					localizations[key].polish = value;
				if (language == "russian")
					localizations[key].russian = value;
				if (language == "simp_chinese")
					localizations[key].simp_chinese = value;
				if (language == "spanish")
					localizations[key].spanish = value;
				if (language == "turkish")
					localizations[key].turkish = value;
			}
			else
			{
				LocBlock newBlock;
				if (language == "braz_por")
					newBlock.english = value;
				if (language == "english")
					newBlock.english = value;
				if (language == "french")
					newBlock.french = value;
				if (language == "german")
					newBlock.german = value;
				if (language == "japanese")
					newBlock.japanese = value;
				if (language == "korean")
					newBlock.korean = value;
				if (language == "polish")
					newBlock.polish = value;
				if (language == "russian")
					newBlock.russian = value;
				if (language == "simp_chinese")
					newBlock.simp_chinese = value;
				if (language == "spanish")
					newBlock.spanish = value;
				if (language == "turkish")
					newBlock.turkish = value;
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

	if (!keyItr->second.english.empty() && (keyItr->second.spanish.empty() || keyItr->second.german.empty() || keyItr->second.french.empty() ||
															 keyItr->second.korean.empty() || keyItr->second.russian.empty() || keyItr->second.simp_chinese.empty()))
	{
		auto newBlock = keyItr->second;
		if (newBlock.spanish.empty())
			newBlock.spanish = newBlock.english;
		if (newBlock.german.empty())
			newBlock.german = newBlock.english;
		if (newBlock.french.empty())
			newBlock.french = newBlock.english;
		if (newBlock.korean.empty())
			newBlock.korean = newBlock.english;
		if (newBlock.russian.empty())
			newBlock.russian = newBlock.english;
		if (newBlock.simp_chinese.empty())
			newBlock.simp_chinese = newBlock.english;
		return newBlock;
	}

	// either all is well, or we're missing english. Can't do anything about the latter. Since English is absolute base, we bail if it's missing.
	// (Quite a few locs have only russian entries and no other languages.)
	if (keyItr->second.english.empty())
		return std::nullopt;

	return keyItr->second;
}

std::optional<std::string> EU5::EU5LocalizationLoader::reverseLookupCultureName(const std::string& localization) const
{
	// This looks for specifically *_name keys cultures use.

	for (const auto& [locName, locBlock]: localizations)
	{
		if (locBlock.english == localization || locBlock.french == localization || locBlock.german == localization || locBlock.korean == localization ||
			 locBlock.russian == localization || locBlock.simp_chinese == localization || locBlock.spanish == localization)
			if (locName.find("_name") != std::string::npos)
				return locName;
	}
	return std::nullopt;
}

std::set<std::string> EU5::EU5LocalizationLoader::reverseLookup(const std::string& localization) const
{
	// This is a general lookup for a loc key.
	std::set<std::string> toReturn;

	for (const auto& [locName, locBlock]: localizations)
	{
		if (locBlock.english == localization || locBlock.french == localization || locBlock.german == localization || locBlock.korean == localization ||
			 locBlock.russian == localization || locBlock.simp_chinese == localization || locBlock.spanish == localization)
			toReturn.emplace(locName);
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