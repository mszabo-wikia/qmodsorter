#include "DependencyDatabaseLoader.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

void DependencyDatabaseLoader::loadDependencyDatabase(
    ModManifestList &modManifestList) {
  QFile dbFile(settings.getDatabasePath());
  if (!dbFile.open(QFile::ReadOnly)) {
    return;
  }

  auto fileContents = dbFile.readAll();
  auto json = QJsonDocument::fromJson(fileContents).object();

  if (json.contains("rules") && json["rules"].isObject()) {
    auto rulesObject = json["rules"].toObject();
    for (const auto &packageId : rulesObject.keys()) {
      // Don't bother loading rules pertaining to mods we don't know about
      if (!rulesObject[packageId].isObject() ||
          !modManifestList.hasModWithPackageId(packageId)) {
        continue;
      }

      auto modObject = rulesObject[packageId].toObject();
      addLoadBeforeRules(modManifestList, packageId, modObject);
      addLoadAfterRules(modManifestList, packageId, modObject);
      addLoadBottomRules(modManifestList, packageId, modObject);
    }
  }
}

void DependencyDatabaseLoader::addLoadAfterRules(
    ModManifestList &modManifestList, const QString &packageId,
    const QJsonObject &modObject) {
  if (modObject["loadAfter"].isObject()) {
    auto loadAfterRules = modObject["loadAfter"].toObject();
    for (const auto &dependencyPackageId : loadAfterRules.keys()) {
      if (modManifestList.hasModWithPackageId(dependencyPackageId)) {
        modManifestList.addModDependency(dependencyPackageId, packageId);
      }
    }
  }
}

void DependencyDatabaseLoader::addLoadBeforeRules(
    ModManifestList &modManifestList, const QString &packageId,
    const QJsonObject &modObject) {
  if (modObject["loadBefore"].isObject()) {
    auto loadBeforeRules = modObject["loadBefore"].toObject();
    for (const auto &dependentPackageId : loadBeforeRules.keys()) {
      if (modManifestList.hasModWithPackageId(dependentPackageId)) {
        modManifestList.addModDependency(packageId, dependentPackageId);
      }
    }
  }
}

void DependencyDatabaseLoader::addLoadBottomRules(
    ModManifestList &modManifestList, const QString &packageId,
    const QJsonObject &modObject) {
  if (modObject["loadBottom"].isObject()) {
    auto loadBottomObject = modObject["loadBottom"].toObject();
    if (loadBottomObject["value"].isBool() &&
        loadBottomObject["value"].toBool()) {
      for (const auto &dependentPackageId : modManifestList.getPackageIds()) {
        if (dependentPackageId != packageId) {
          modManifestList.addModDependency(dependentPackageId, packageId);
        }
      }
    }
  }
}
