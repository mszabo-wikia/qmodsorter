#ifndef MOD_MANIFEST_HPP
#define MOD_MANIFEST_HPP

#include <QString>

/**
 * @brief Holds metadata pertaining to a single mod.
 */
class ModManifest {
 public:
  ModManifest() {}
  const QString &getName() const { return name; }
  const QString &getPackageId() const { return packageId; }
  void setName(QString name) { this->name = name; }
  void setPackageId(QString packageId) { this->packageId = packageId; }

 private:
  QString name;
  QString packageId;
};

#endif
