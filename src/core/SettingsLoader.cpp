#include "SettingsLoader.hpp"

#include <QDir>

namespace qmodsorter {

SettingsLoader SettingsLoader::newWithDefaultFolder() {
  auto defaultSettingsFolder = fs::path(QDir::homePath().toStdString()) /
                               ".local" / "share" / "qmodsorter";
  return SettingsLoader(defaultSettingsFolder);
}

void SettingsLoader::loadSettings(Settings &settings) {
  fs::create_directories(settingsFolder);

  QFile settingsFile(settingsFolder / "settings.xml");
  if (!settingsFile.open(QFile::ReadOnly | QFile::ExistingOnly)) {
    return;
  }
  xmlReader.setDevice(&settingsFile);

  if (!xmlReader.readNextStartElement() ||
      xmlReader.name() != QLatin1String("settings")) {
    return;
  }

  while (xmlReader.readNextStartElement()) {
    if (xmlReader.name() == QLatin1String("gameInstallPath")) {
      settings.setGameInstallPath(xmlReader.readElementText());
    } else if (xmlReader.name() == QLatin1String("workshopPath")) {
      settings.setWorkshopPath(xmlReader.readElementText());
    } else if (xmlReader.name() == QLatin1String("databasePath")) {
      settings.setDatabasePath(xmlReader.readElementText());
    } else {
      xmlReader.skipCurrentElement();
    }
  }
}

void SettingsLoader::saveSettings(const Settings &settings) {
  fs::create_directories(settingsFolder);

  QFile settingsFile(settingsFolder / "settings.xml");
  settingsFile.open(QFile::WriteOnly);
  xmlWriter.setDevice(&settingsFile);
  xmlWriter.setAutoFormatting(true);

  xmlWriter.writeStartDocument();
  xmlWriter.writeStartElement("settings");
  xmlWriter.writeTextElement("gameInstallPath", settings.getGameInstallPath());
  xmlWriter.writeTextElement("workshopPath", settings.getWorkshopPath());
  xmlWriter.writeTextElement("databasePath", settings.getDatabasePath());
  xmlWriter.writeEndDocument();
}

}  // namespace qmodsorter
