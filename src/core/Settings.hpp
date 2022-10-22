#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QString>

/**
 * @brief Holds application-specific configuration options.
 */
class Settings {
 public:
  QString getGameInstallPath() const { return gameInstallPath; }
  QString getDatabasePath() const { return databasePath; }
  QString getWorkshopPath() const { return workshopPath; }
  void setGameInstallPath(QString gameInstallPath) {
    this->gameInstallPath = gameInstallPath;
  }
  void setDatabasePath(QString databasePath) {
    this->databasePath = databasePath;
  }
  void setWorkshopPath(QString workshopPath) {
    this->workshopPath = workshopPath;
  }

 private:
  QString gameInstallPath;
  QString databasePath;
  QString workshopPath;
};

#endif
