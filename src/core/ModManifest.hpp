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
  const QString &getName() const noexcept { return name; }
  const QString &getPackageId() const noexcept { return packageId; }
  const fs::path &getFolder() const noexcept { return folder; }
  const QString &getDescription() const noexcept { return description; }
  void setName(QString name) noexcept { this->name = name; }
  void setPackageId(QString packageId) noexcept { this->packageId = packageId; }
  void setFolder(const fs::path folder) noexcept { this->folder = folder; }
  void setDescription(QString description) noexcept {
    this->description = description;
  }

 private:
  QString name;
  QString packageId;
  QString description;
  fs::path folder;
};

#endif
