#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QString>

/**
 * @brief Holds application-specific configuration options.
 */
class Settings {
 public:
  QString getGameInstallPath() const noexcept { return gameInstallPath; }
  QString getDatabasePath() const noexcept { return databasePath; }
  QString getWorkshopPath() const noexcept { return workshopPath; }
  void setGameInstallPath(QString gameInstallPath) noexcept {
    this->gameInstallPath = gameInstallPath;
  }
  void setDatabasePath(QString databasePath) noexcept {
    this->databasePath = databasePath;
  }
  void setWorkshopPath(QString workshopPath) noexcept {
    this->workshopPath = workshopPath;
  }

 private:
  QString gameInstallPath;
  QString databasePath;
  QString workshopPath;
};

#endif
