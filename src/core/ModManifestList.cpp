#include "ModManifestList.hpp"

#include "ModManifest.hpp"

namespace qmodsorter {

const ModManifest ModManifestList::getModByName(const QString &name) const {
  return modsByName.value(name);
}

const ModManifest ModManifestList::getModByPackageId(
    const QString &packageId) const {
  return modsByPackageId.value(packageId);
}

const QStringList ModManifestList::getPackageIds() const {
  return modsByPackageId.keys();
}

bool ModManifestList::hasModNamed(const QString &name) const {
  return modsByName.contains(name);
}

bool ModManifestList::hasModWithPackageId(const QString &packageId) const {
  return modsByPackageId.contains(packageId);
}

const QMap<QString, ModManifest> &ModManifestList::getMods() const noexcept {
  return modsByName;
}

void ModManifestList::addModManifest(ModManifest modManifest) {
  modsByName[modManifest.getName()] = modManifest;
  modsByPackageId[modManifest.getPackageId()] = modManifest;
}

void ModManifestList::addModDependency(QString dependency, QString dependent) {
  dependencies.insert(std::make_pair(dependency, dependent));
}

const std::set<std::pair<QString, QString>> &ModManifestList::getDependencies()
    const noexcept {
  return dependencies;
}

void ModManifestList::clear() {
  modsByName.clear();
  modsByPackageId.clear();
  dependencies.clear();
}

}  // namespace qmodsorter
