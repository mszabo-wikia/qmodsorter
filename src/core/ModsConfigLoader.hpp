#ifndef MODS_CONFIG_LOADER_HPP
#define MODS_CONFIG_LOADER_HPP

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <filesystem>

#include "ModsConfig.hpp"

namespace fs = std::filesystem;

/**
 * @brief Loads the active modlist and game info (i.e. the ModsConfig.xml file).
 *
 */
class ModsConfigLoader {
 public:
  ModsConfigLoader(const fs::path &modsConfigPath)
      : modsConfigPath(modsConfigPath) {}

  void loadModsConfig(ModsConfig &modsConfig);
  void saveModsConfig(const ModsConfig &modsConfig);
  /**
   * @brief Obtain a ModsConfigLoader instance configured to use the
   * platform-specific default config path.
   *
   * @return ModsConfigLoader
   */
  static ModsConfigLoader newWithDefaultConfigPath();

 private:
  fs::path modsConfigPath;
  QXmlStreamReader xmlReader;
  QXmlStreamWriter xmlWriter;

  void readActiveMods(ModsConfig &modsConfig);
  void readKnownExpansions(ModsConfig &modsConfig);
};

#endif
