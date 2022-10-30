#ifndef SETTINGS_LOADER_HPP
#define SETTINGS_LOADER_HPP

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <filesystem>

#include "Settings.hpp"

namespace qmodsorter {

namespace fs = std::filesystem;

/**
 * @brief Loader for application-specific settings.
 */
class SettingsLoader {
 public:
  SettingsLoader(const fs::path &settingsFolder)
      : settingsFolder(settingsFolder) {}
  void loadSettings(Settings &settings);
  void saveSettings(const Settings &settings);
  /**
   * @brief Get a SettingsLoader instance configured to use the
   * platform-specific default settings folder.
   *
   * @return SettingsLoader
   */
  static SettingsLoader newWithDefaultFolder();

 private:
  fs::path settingsFolder;
  QXmlStreamReader xmlReader;
  QXmlStreamWriter xmlWriter;
};

}  // namespace qmodsorter

#endif
