#ifndef MOD_MANIFEST_LIST_HPP
#define MOD_MANIFEST_LIST_HPP

#include <QMap>
#include <QString>
#include <set>

#include "ModManifest.hpp"

/**
 * @brief Holds all known mods and their dependency information.
 */
class ModManifestList {
 public:
  /**
   * @brief Get the manifest for the mod with the given name.
   *
   * @param name name of the mod as specified in its manifest file
   * @return const ModManifest
   */
  const ModManifest getModByName(const QString &name) const;
  /**
   * @brief Get the manifest for the mod with the given package ID.
   *
   * @param packageId lower-case package ID of the mod
   * @return const ModManifest
   */
  const ModManifest getModByPackageId(const QString &packageId) const;
  /**
   * @brief Get a read-only list of the package IDs of all known mods.
   *
   * @return const QStringList
   */
  const QStringList getPackageIds() const;
  /**
   * @brief Determine whether the set of known mods contains any mods with the
   * given name.
   *
   * @param name name of the mod as specified in its manifest file
   * @return bool
   */
  bool hasModNamed(const QString &name) const;
  /**
   * @brief Determine whether the set of known mods contains any mods with the
   * given package ID.
   *
   * @param packageId lower-case package ID of the mod
   * @return bool
   */
  bool hasModWithPackageId(const QString &packageId) const;
  /**
   * @brief Get a read-only view of all known mods keyed by mod name.
   *
   * @return const QMap<QString, ModManifest>&
   */
  const QMap<QString, ModManifest> &getMods();
  /**
   * @brief Add a new mod to the set of known mods.
   *
   * @param modManifest
   */
  void addModManifest(ModManifest modManifest);
  /**
   * @brief Add a dependency rule between two mods.
   *
   * @param dependency lower-case package ID of the dependency mod
   * @param dependent lower-case package ID of the dependent mod
   */
  void addModDependency(QString dependency, QString dependent);
  /**
   * @brief Get a read-only view of all dependency rules between known mods.
   *
   * @return const std::set<std::pair<QString, QString>>&
   */
  const std::set<std::pair<QString, QString>> &getDependencies() const;

  void clear();

 private:
  QMap<QString, ModManifest> modsByName;
  QMap<QString, ModManifest> modsByPackageId;
  std::set<std::pair<QString, QString>> dependencies;
};

#endif
