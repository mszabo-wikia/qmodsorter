#ifndef MODS_CONFIG_HPP
#define MODS_CONFIG_HPP

#include <QList>
#include <QString>

class ModsConfig {
 public:
  QString getVersion() const noexcept { return version; }
  const QStringList &getActiveMods() const noexcept { return activeMods; }
  const QStringList &getKnownExpansions() const noexcept {
    return knownExpansions;
  };

  void setVersion(QString version) noexcept { this->version = version; }
  void setActiveMods(QStringList activeMods) noexcept {
    this->activeMods = activeMods;
  }
  void setKnownExpansions(QStringList knownExpansions) noexcept {
    this->knownExpansions = knownExpansions;
  }

 private:
  QString version;
  QStringList activeMods;
  QStringList knownExpansions;
};

#endif
