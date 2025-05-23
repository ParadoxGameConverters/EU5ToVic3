#include "Configuration.h"
#include "gtest/gtest.h"

TEST(ConfigurationTests, BrokenEU4PathThrowsException)
{
	std::stringstream configurationInput;
	configurationInput << "EU5Directory = \"TestFiles/eu5installationBROKE\"\n";
	configurationInput << "Vic3Directory = \"TestFiles/vic3installation\"\n";
	const commonItems::ConverterVersion converterVersion;

	EXPECT_THROW(const auto configuration = Configuration(configurationInput, converterVersion), std::runtime_error);
}

TEST(ConfigurationTests, BrokenVic3PathThrowsException)
{
	std::stringstream configurationInput;
	configurationInput << "EU5Directory = \"TestFiles/eu5installation\"\n";
	configurationInput << "Vic3Directory = \"TestFiles/vic3installationBROKE\"\n";
	const commonItems::ConverterVersion converterVersion;

	EXPECT_THROW(const auto configuration = Configuration(configurationInput, converterVersion), std::runtime_error);
}

TEST(ConfigurationTests, InstallationPathsCanBeUpdatedRetrieved)
{
	std::stringstream configurationInput;
	configurationInput << "EU5Directory = \"TestFiles/eu5installation\"\n";
	configurationInput << "Vic3Directory = \"TestFiles/vic3installation\"\n"; // updated with "/game/"
	const commonItems::ConverterVersion converterVersion;
	const auto configuration = Configuration(configurationInput, converterVersion);

	EXPECT_EQ(std::filesystem::path("TestFiles/eu5installation"), configuration.getEU5Path());
	EXPECT_EQ(std::filesystem::path("TestFiles/vic3installation/game"), configuration.getVic3Path());
}

TEST(ConfigurationTests, SaveAndDocumentsPathCanBeRetrieved)
{
	std::stringstream configurationInput;
	configurationInput << "EU5Directory = \"TestFiles/eu5installation\"\n";
	configurationInput << "Vic3Directory = \"TestFiles/vic3installation\"\n";
	configurationInput << "EU5DocumentsDirectory = \"TestFiles\"\n";
	configurationInput << "SaveGame = \"C:\\autosave.eu5\"\n";
	const commonItems::ConverterVersion converterVersion;
	const auto configuration = Configuration(configurationInput, converterVersion);

	EXPECT_EQ(std::filesystem::path("TestFiles"), configuration.getEU5DocumentsPath());
#ifdef _MSC_BUILD
	EXPECT_EQ(std::filesystem::path("C:\\autosave.eu5"), configuration.getEU5SaveGamePath());
#else
	EXPECT_EQ(std::filesystem::path("C__autosave.eu5"), configuration.getEU5SaveGamePath());
#endif
}

TEST(ConfigurationTests, OutputNameNormalizesSetsFromSavegameName)
{
	std::stringstream configurationInput;
	configurationInput << "EU5Directory = \"TestFiles/eu5installation\"\n";
	configurationInput << "Vic3Directory = \"TestFiles/vic3installation\"\n";
	configurationInput << "SaveGame = \"C:\\autosave.eu5\"\n";
	const commonItems::ConverterVersion converterVersion;
	const auto configuration = Configuration(configurationInput, converterVersion);

	EXPECT_EQ(std::filesystem::path("autosave"), configuration.getOutputName());
}

TEST(ConfigurationTests, OutputNameNormalizesItselfFromSavegameName)
{
	std::stringstream configurationInput;
	configurationInput << "EU5Directory = \"TestFiles/eu5installation\"\n";
	configurationInput << "Vic3Directory = \"TestFiles/vic3installation\"\n";
	configurationInput << "SaveGame = \"C:\\autosave - something.eu5\"\n";
	const commonItems::ConverterVersion converterVersion;
	const auto configuration = Configuration(configurationInput, converterVersion);

#ifdef _MSC_BUILD
	EXPECT_EQ(std::filesystem::path("autosave___something"), configuration.getOutputName());
#else
	EXPECT_EQ(std::filesystem::path("C__autosave___something"), configuration.getOutputName());
#endif
}

TEST(ConfigurationTests, OutputNameSetsFromOverrideName)
{
	std::stringstream configurationInput;
	configurationInput << "EU5Directory = \"TestFiles/eu5installation\"\n";
	configurationInput << "Vic3Directory = \"TestFiles/vic3installation\"\n";
	configurationInput << "SaveGame = \"C:\\autosave.eu5\"\n";
	configurationInput << "output_name = \"ddd\"\n";
	const commonItems::ConverterVersion converterVersion;
	const auto configuration = Configuration(configurationInput, converterVersion);

	EXPECT_EQ(std::filesystem::path("ddd"), configuration.getOutputName());
}

TEST(ConfigurationTests, OutputNameNormalizesItselfFromOverrideName)
{
	std::stringstream configurationInput;
	configurationInput << "EU5Directory = \"TestFiles/eu5installation\"\n";
	configurationInput << "Vic3Directory = \"TestFiles/vic3installation\"\n";
	configurationInput << "SaveGame = \"C:\\autosave - something.eu5\"\n";
	configurationInput << "output_name = \"ddd - something\"\n";
	const commonItems::ConverterVersion converterVersion;
	const auto configuration = Configuration(configurationInput, converterVersion);

	EXPECT_EQ(std::filesystem::path("ddd___something"), configuration.getOutputName());
}
