#include <SettingsLoader.hpp>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>

TEST_CASE("SettingsLoader can load and save settings", "[SettingsLoader]") {
  auto settingsPath = fs::current_path() / "fixtures";

  SECTION("round-trip") {
    SettingsLoader settingsLoader(settingsPath);
    Settings settings;
    settings.setDatabasePath("/path/to/communityRules.json");
    settings.setGameInstallPath("/path/to/RimWorld");
    settings.setWorkshopPath("/path/to/workshop");

    Settings newSettings;

    settingsLoader.saveSettings(settings);
    settingsLoader.loadSettings(newSettings);

    REQUIRE(newSettings.getGameInstallPath().toStdString() ==
            "/path/to/RimWorld");
    REQUIRE(newSettings.getWorkshopPath().toStdString() == "/path/to/workshop");
    REQUIRE(newSettings.getDatabasePath().toStdString() ==
            "/path/to/communityRules.json");
  }

  fs::remove(settingsPath / "settings.xml");
}
