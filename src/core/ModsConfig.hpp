#ifndef MODS_CONFIG_HPP
#define MODS_CONFIG_HPP

#include <QList>
#include <QString>

class ModsConfig {
 public:
  QString getVersion() const { return version; }
  const QStringList &getActiveMods() const { return activeMods; }
  const QStringList &getKnownExpansions() const { return knownExpansions; };

  void setVersion(QString version) { this->version = version; }
  void setActiveMods(QStringList activeMods) { this->activeMods = activeMods; }
  void setKnownExpansions(QStringList knownExpansions) {
    this->knownExpansions = knownExpansions;
  }

 private:
  QString version;
  QStringList activeMods;
  QStringList knownExpansions;
};

#endif
