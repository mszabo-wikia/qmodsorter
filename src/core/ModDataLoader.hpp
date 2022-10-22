#ifndef ABOUT_XML_PARSER_HPP
#define ABOUT_XML_PARSER_HPP

#include <QFile>
#include <QXmlStreamReader>
#include <filesystem>
#include <set>
#include <vector>

#include "ModManifest.hpp"
#include "ModManifestList.hpp"

namespace fs = std::filesystem;

/**
 * @brief Loader class for manifest data for mods
 */
class ModDataLoader {
 public:
  /**
   * @brief Load manifest information (metadata and bundled load order rules)
   * for all mods in the given folder.
   *
   * @param ModManifestList modlist data to populate
   * @param modsFolder folder to scan for mods
   */
  void loadMods(ModManifestList &ModManifestList, const fs::path &modsFolder);

 private:
  QXmlStreamReader xml;
  void readAboutXml(const fs::path &modFolder,
                    ModManifestList &ModManifestList);
  void readLoadOrderRules(std::vector<QString> &loadOrderRules);
};
#endif
