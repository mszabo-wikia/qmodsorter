#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <ModDataLoader.hpp>
#include <ModManifest.hpp>
#include <ModManifestList.hpp>
#include <ModNameListModel.hpp>
#include <ModsConfig.hpp>
#include <ModsConfigLoader.hpp>
#include <QListView>
#include <QPushButton>
#include <QTabWidget>
#include <QWidget>
#include <Settings.hpp>
#include <SettingsLoader.hpp>

#include "ModManifest.hpp"

namespace qmodsorter {

class MainWindow : public QWidget {
  Q_OBJECT

 public:
  MainWindow(Settings &settings, ModsConfig &modsConfig,
             ModsConfigLoader &modsConfigLoader);

 private slots:
  void onRefreshButtonClicked();
  void onSortButtonClicked();
  void onSaveButtonClicked();
  void onModClickedInList(const QModelIndex &selected);

 signals:
  void selectedModChanged(const ModManifest &modManifest);

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

  QTabWidget *headerTabs;
  int modDetailsTabIndex;

  void moveSelectedItems(QListView *sourceView, QListView *targetView);
  void moveAllItems(QListView *sourceView, QListView *targetView);
};

}  // namespace qmodsorter

#endif
