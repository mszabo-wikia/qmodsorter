#include "ModsConfigLoader.hpp"

#include <QDir>
#include <QFile>

#include "ModsConfig.hpp"

ModsConfigLoader ModsConfigLoader::newWithDefaultConfigPath() {
  // TODO: Should this be configurable?
  // AFAIK there is no way to have the game use another path.
  auto defaultConfigPath = fs::path(QDir::homePath().toStdString()) /
                           ".config" / "unity3d" / "Ludeon Studios" /
                           "RimWorld by Ludeon Studios" / "Config" /
                           "ModsConfig.xml";
  return ModsConfigLoader(defaultConfigPath);
}

void ModsConfigLoader::loadModsConfig(ModsConfig &modsConfig) {
  QFile modsConfigFile(modsConfigPath);

  modsConfigFile.open(QFile::ReadOnly);
  xmlReader.setDevice(&modsConfigFile);

  if (!xmlReader.readNextStartElement() ||
      xmlReader.name() != QLatin1String("ModsConfigData")) {
    return;
  }

  while (xmlReader.readNextStartElement()) {
    if (xmlReader.name() == QLatin1String("version")) {
      modsConfig.setVersion(xmlReader.readElementText());
    } else if (xmlReader.name() == QLatin1String("activeMods")) {
      readActiveMods(modsConfig);
    } else if (xmlReader.name() == QLatin1String("knownExpansions")) {
      readKnownExpansions(modsConfig);
    } else {
      xmlReader.skipCurrentElement();
    }
  }
}

void ModsConfigLoader::readActiveMods(ModsConfig &modsConfig) {
  QStringList activeMods;

  while (xmlReader.readNextStartElement()) {
    if (xmlReader.name() == QLatin1String("li")) {
      activeMods.push_back(xmlReader.readElementText());
    } else {
      xmlReader.skipCurrentElement();
    }
  }

  modsConfig.setActiveMods(activeMods);
}

void ModsConfigLoader::readKnownExpansions(ModsConfig &modsConfig) {
  QStringList knownExpansions;

  while (xmlReader.readNextStartElement()) {
    if (xmlReader.name() == QLatin1String("li")) {
      knownExpansions.push_back(xmlReader.readElementText());
    } else {
      xmlReader.skipCurrentElement();
    }
  }

  modsConfig.setKnownExpansions(knownExpansions);
}

void ModsConfigLoader::saveModsConfig(const ModsConfig &modsConfig) {
  QFile modsConfigFile(modsConfigPath);

  modsConfigFile.open(QFile::WriteOnly);
  xmlWriter.setDevice(&modsConfigFile);
  xmlWriter.setAutoFormatting(true);

  xmlWriter.writeStartDocument();
  xmlWriter.writeStartElement("ModsConfigData");
  xmlWriter.writeTextElement("version", modsConfig.getVersion());

  xmlWriter.writeStartElement("activeMods");
  for (const auto &activeModId : modsConfig.getActiveMods()) {
    xmlWriter.writeTextElement("li", activeModId);
  }
  xmlWriter.writeEndElement();

  xmlWriter.writeStartElement("knownExpansions");
  for (const auto &knownExpansionId : modsConfig.getKnownExpansions()) {
    xmlWriter.writeTextElement("li", knownExpansionId);
  }
  xmlWriter.writeEndElement();

  xmlWriter.writeEndDocument();
}
