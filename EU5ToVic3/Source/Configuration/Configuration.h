#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include "ConverterVersion.h"
#include "Parser.h"

class Configuration: commonItems::parser
{
  public:
	Configuration() = default;
	explicit Configuration(const commonItems::ConverterVersion& converterVersion);
	explicit Configuration(std::istream& theStream, const commonItems::ConverterVersion& converterVersion);

	enum class STARTDATE
	{
		Vanilla = 1,
		Dynamic = 2
	};

	struct ConfigBlock
	{
		STARTDATE startDate = STARTDATE::Vanilla;
	} configBlock;

	void setOutputName(const std::filesystem::path& name) { outputName = name; }
	void setVanillaStartDate() { configBlock.startDate = STARTDATE::Vanilla; }

	[[nodiscard]] const auto& getEU5SaveGamePath() const { return EU5SaveGamePath; }
	[[nodiscard]] const auto& getEU5Path() const { return EU5Path; }
	[[nodiscard]] const auto& getEU5DocumentsPath() const { return EU5DocumentsPath; }
	[[nodiscard]] const auto& getVic3Path() const { return Vic3Path; }
	[[nodiscard]] const auto& getOutputName() const { return outputName; }
	[[nodiscard]] const auto& getEU5Version() const { return EU5Version; }
	[[nodiscard]] const auto& getVic3Version() const { return Vic3Version; }

  private:
	void registerKeys();
	void verifyEU5Path();
	void verifyVic3Path();
	void setOutputName();
	void verifyVic3Version(const commonItems::ConverterVersion& converterVersion);
	void verifyEU5Version(const commonItems::ConverterVersion& converterVersion);

	// options from configuration.txt
	std::filesystem::path EU5SaveGamePath;
	std::filesystem::path EU5Path;
	std::filesystem::path EU5DocumentsPath;
	std::filesystem::path Vic3Path;
	std::filesystem::path outputName;
	std::optional<GameVersion> EU5Version;
	std::optional<GameVersion> Vic3Version;
};

#endif // CONFIGURATION_H
