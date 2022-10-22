#include "ModDataLoader.hpp"

#include "ModManifest.hpp"
#include "ModManifestList.hpp"

void ModDataLoader::loadMods(ModManifestList &modManifestList,
                             const fs::path &modsFolder) {
  for (const auto &entry : fs::directory_iterator(modsFolder)) {
    if (entry.is_directory()) {
      readAboutXml(entry.path(), modManifestList);
    }
  }
}

void ModDataLoader::readAboutXml(const fs::path &modFolder,
                                 ModManifestList &modManifestList) {
  QFile aboutXml(modFolder / "About" / "About.xml");
  if (!aboutXml.open(QFile::ReadOnly)) {
    return;
  }

  xml.setDevice(&aboutXml);

  if (!xml.readNextStartElement() ||
      xml.name() != QLatin1String("ModMetaData")) {
    return;
  }

  ModManifest modManifest;
  std::vector<QString> loadAfter;
  std::vector<QString> loadBefore;

  while (xml.readNextStartElement()) {
    if (xml.name() == QLatin1String("name")) {
      modManifest.setName(xml.readElementText());
    } else if (xml.name() == QLatin1String("packageId")) {
      auto packageId = xml.readElementText();
      modManifest.setPackageId(packageId.toLower());
    } else if (xml.name() == QLatin1String("loadAfter")) {
      readLoadOrderRules(loadAfter);
    } else if (xml.name() == QLatin1String("loadBefore")) {
      readLoadOrderRules(loadBefore);
    } else {
      xml.skipCurrentElement();
    }
  }

  if (modManifest.getPackageId().isEmpty()) {
    return;
  }

  // Some mods, such as Ludeon's own DLCs and core, may not have a name
  // specified. Fall back to the directory name for this case (and hope it's
  // something sensible).
  if (modManifest.getName().isEmpty()) {
    modManifest.setName(modFolder.filename().c_str());
  }

  for (const auto dependency : loadAfter) {
    modManifestList.addModDependency(dependency.toLower(),
                                     modManifest.getPackageId());
  }

  for (const auto dependent : loadBefore) {
    modManifestList.addModDependency(modManifest.getPackageId(),
                                     dependent.toLower());
  }

  modManifestList.addModManifest(modManifest);
}

void ModDataLoader::readLoadOrderRules(std::vector<QString> &loadOrderRules) {
  while (xml.readNextStartElement()) {
    if (xml.name() == QLatin1String("li")) {
      loadOrderRules.push_back(xml.readElementText());
    } else {
      xml.skipCurrentElement();
    }
  }
}
