#ifndef MOD_MANIFEST_HPP
#define MOD_MANIFEST_HPP

#include <QString>
#include <filesystem>

namespace fs = std::filesystem;

/**
 * @brief Holds metadata pertaining to a single mod.
 */
class ModManifest {
 public:
  ModManifest() {}
  const QString &getName() const { return name; }
  const QString &getPackageId() const { return packageId; }
  const fs::path &getFolder() const { return folder; }
  const QString &getDescription() const { return description; }
  void setName(QString name) { this->name = name; }
  void setPackageId(QString packageId) { this->packageId = packageId; }
  void setFolder(const fs::path folder) { this->folder = folder; }
  void setDescription(QString description) { this->description = description; }

 private:
  QString name;
  QString packageId;
  QString description;
  fs::path folder;
};

#endif
