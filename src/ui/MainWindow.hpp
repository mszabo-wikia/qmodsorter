#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <ModDataLoader.hpp>
#include <ModManifestList.hpp>
#include <ModNameListModel.hpp>
#include <ModsConfig.hpp>
#include <ModsConfigLoader.hpp>
#include <QListView>
#include <QPushButton>
#include <QWidget>
#include <Settings.hpp>
#include <SettingsLoader.hpp>

class MainWindow : public QWidget {
  Q_OBJECT

 public:
  MainWindow(Settings &settings, ModsConfig &modsConfig,
             ModsConfigLoader &modsConfigLoader);

 private slots:
  void onRefreshButtonClicked();
  void onSortButtonClicked();
  void onSaveButtonClicked();

 private:
  Settings &settings;
  ModsConfig &modsConfig;
  ModsConfigLoader &modsConfigLoader;
  ModDataLoader modDataLoader;
  ModManifestList mods;
  ModNameListModel *enabledModsModel;
  ModNameListModel *disabledModsModel;
  QListView *enabledModsView;
  QListView *disabledModsView;
  QPushButton *refreshButton;
  QPushButton *sortButton;
  QPushButton *saveButton;

  void moveSelectedItems(QListView *sourceView, QListView *targetView);
  void moveAllItems(QListView *sourceView, QListView *targetView);
};

#endif
