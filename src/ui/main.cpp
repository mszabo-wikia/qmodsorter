#include <ModsConfig.hpp>
#include <ModsConfigLoader.hpp>
#include <QApplication>
#include <Settings.hpp>
#include <SettingsLoader.hpp>

#include "MainWindow.hpp"

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  Settings settings;
  ModsConfig modsConfig;

  auto settingsLoader = SettingsLoader::newWithDefaultFolder();
  auto modsConfigLoader = ModsConfigLoader::newWithDefaultConfigPath();

  settingsLoader.loadSettings(settings);
  modsConfigLoader.loadModsConfig(modsConfig);

  MainWindow mainWindow(settings, modsConfig, modsConfigLoader);
  mainWindow.show();

  // Persist our application settings on exit
  QObject::connect(&app, &QApplication::aboutToQuit,
                   [&]() { settingsLoader.saveSettings(settings); });

  return app.exec();
}
