#ifndef DEPENDENCY_DATABASE_LOADER_HPP
#define DEPENDENCY_DATABASE_LOADER_HPP

#include <QJsonObject>

#include "ModManifestList.hpp"
#include "Settings.hpp"

namespace qmodsorter {

/**
 * @brief Handles loading dependency information from RimPy's community rules
 * file.
 */
class DependencyDatabaseLoader {
 public:
  DependencyDatabaseLoader(const Settings &settings) : settings(settings) {}
  /**
   * @brief Populate the given mod manifest holder with dependency data.
   *
   * @param modManifestList
   */
  void loadDependencyDatabase(ModManifestList &modManifestList);

 private:
  const Settings &settings;

  void addLoadAfterRules(ModManifestList &modManifestList,
                         const QString &packageId,
                         const QJsonObject &modObject);
  void addLoadBeforeRules(ModManifestList &modManifestList,
                          const QString &packageId,
                          const QJsonObject &modObject);
  void addLoadBottomRules(ModManifestList &modManifestList,
                          const QString &packageId,
                          const QJsonObject &modObject);
};

}  // namespace qmodsorter
#endif
