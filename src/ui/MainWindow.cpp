#include "MainWindow.hpp"

#include <CircularDependencies.hpp>
#include <DependencyDatabaseLoader.hpp>
#include <DependencyGraph.hpp>
#include <ModManifest.hpp>
#include <ModManifestList.hpp>
#include <ModNameListModel.hpp>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QList>
#include <QListView>
#include <QMessageBox>
#include <QPushButton>
#include <QStringListModel>
#include <QVBoxLayout>

#include "CircularDependenciesDialog.hpp"
#include "ModDetailsPanel.hpp"
#include "PathSelectorPanel.hpp"

namespace qmodsorter {

MainWindow::MainWindow(Settings &settings, ModsConfig &modsConfig,
                       ModsConfigLoader &modsConfigLoader)
    : settings(settings),
      modsConfig(modsConfig),
      modsConfigLoader(modsConfigLoader) {
  disabledModsModel = new ModNameListModel(this);

  disabledModsView = new QListView();
  disabledModsView->setSelectionMode(QListView::ExtendedSelection);
  disabledModsView->setModel(disabledModsModel);
  disabledModsView->setDragEnabled(true);
  disabledModsView->setAcceptDrops(true);
  disabledModsView->setDragDropMode(QListView::InternalMove);
  disabledModsView->setMinimumHeight(500);

  enabledModsModel = new ModNameListModel(this);

  enabledModsView = new QListView();
  enabledModsView->setModel(enabledModsModel);
  enabledModsView->setSelectionMode(QListView::ExtendedSelection);
  enabledModsView->setDragEnabled(true);
  enabledModsView->setAcceptDrops(true);
  enabledModsView->setDragDropMode(QListView::InternalMove);
  enabledModsView->setMinimumHeight(500);

  auto enableSelectedModButton = new QPushButton(">");
  auto enableAllModsButton = new QPushButton(">>");
  auto disableSelectedModButton = new QPushButton("<");
  auto disableAllModsButton = new QPushButton("<<");

  auto buttonLayout = new QVBoxLayout();
  buttonLayout->setSpacing(18);
  buttonLayout->setAlignment(Qt::AlignTop);
  buttonLayout->addWidget(enableSelectedModButton);
  buttonLayout->addWidget(enableAllModsButton);
  buttonLayout->addWidget(disableSelectedModButton);
  buttonLayout->addWidget(disableAllModsButton);

  refreshButton = new QPushButton("Refresh");
  sortButton = new QPushButton("Sort");
  saveButton = new QPushButton("Save");

  auto generalActionsLayout = new QVBoxLayout();
  generalActionsLayout->setSpacing(18);
  generalActionsLayout->setAlignment(Qt::AlignTop);
  generalActionsLayout->addWidget(refreshButton);
  generalActionsLayout->addWidget(sortButton);
  generalActionsLayout->addWidget(saveButton);

  auto modManagementLayout = new QHBoxLayout();
  modManagementLayout->setSpacing(18);
  modManagementLayout->addWidget(disabledModsView);
  modManagementLayout->addLayout(buttonLayout);
  modManagementLayout->addWidget(enabledModsView);
  modManagementLayout->addLayout(generalActionsLayout);
  modManagementLayout->setContentsMargins(0, 18, 0, 0);

  auto gamePathSelector = new PathSelectorPanel(settings.getGameInstallPath());
  gamePathSelector->setDirectorySelector(true);
  auto workshopPathSelector = new PathSelectorPanel(settings.getWorkshopPath());
  workshopPathSelector->setDirectorySelector(true);
  auto dbPathSelector = new PathSelectorPanel(settings.getDatabasePath());

  auto pathSelectorLayout = new QFormLayout();
  pathSelectorLayout->addRow("Game folder", gamePathSelector);
  pathSelectorLayout->addRow("Workshop path", workshopPathSelector);
  pathSelectorLayout->addRow("DB path", dbPathSelector);

  auto pathSelectorContainer = new QWidget();
  pathSelectorContainer->setLayout(pathSelectorLayout);

  auto modDetailsPanel = new ModDetailsPanel();

  headerTabs = new QTabWidget();
  headerTabs->addTab(pathSelectorContainer, "Game locations");
  modDetailsTabIndex = headerTabs->addTab(modDetailsPanel, "Mod details");
  headerTabs->setTabEnabled(modDetailsTabIndex, false);

  auto mainVerticalLayout = new QVBoxLayout();
  mainVerticalLayout->addWidget(headerTabs);
  mainVerticalLayout->addLayout(modManagementLayout);

  setMinimumWidth(1024);
  setLayout(mainVerticalLayout);

  connect(gamePathSelector, &PathSelectorPanel::pathSelected, this,
          [&](const QString &path) { settings.setGameInstallPath(path); });
  connect(workshopPathSelector, &PathSelectorPanel::pathSelected, this,
          [&](const QString &path) { settings.setWorkshopPath(path); });
  connect(dbPathSelector, &PathSelectorPanel::pathSelected, this,
          [&](const QString &path) { settings.setDatabasePath(path); });

  connect(refreshButton, &QPushButton::clicked, this,
          &MainWindow::onRefreshButtonClicked);
  connect(sortButton, &QPushButton::clicked, this,
          &MainWindow::onSortButtonClicked);
  connect(saveButton, &QPushButton::clicked, this,
          &MainWindow::onSaveButtonClicked);

  connect(enableSelectedModButton, &QPushButton::clicked, this,
          [&]() { moveSelectedItems(disabledModsView, enabledModsView); });
  connect(disableSelectedModButton, &QPushButton::clicked, this,
          [&]() { moveSelectedItems(enabledModsView, disabledModsView); });
  connect(enableAllModsButton, &QPushButton::clicked, this,
          [&]() { moveAllItems(disabledModsView, enabledModsView); });
  connect(disableAllModsButton, &QPushButton::clicked, this,
          [&]() { moveAllItems(enabledModsView, disabledModsView); });

  connect(enabledModsView, &QListView::clicked, this,
          &MainWindow::onModClickedInList);
  connect(disabledModsView, &QListView::clicked, this,
          &MainWindow::onModClickedInList);

  connect(this, &MainWindow::selectedModChanged, modDetailsPanel,
          &ModDetailsPanel::onSelectedModChanged);

  onRefreshButtonClicked();
}

void MainWindow::onRefreshButtonClicked() {
  DependencyDatabaseLoader dependencyDatabaseLoader(settings);
  mods.clear();

  refreshButton->setDisabled(true);

  const auto gameInstallPath =
      fs::path(settings.getGameInstallPath().toStdString());

  // While the workshop path is optional (it conceivably need not exist for
  // non-Steam installs), it's not possible to load and sort a functional
  // modlist without a valid game folder and rules DB file.
  // So, notify the user if either of those are absent.
  if (!fs::is_directory(gameInstallPath) ||
      !fs::is_regular_file(settings.getDatabasePath().toStdString())) {
    QMessageBox missingSettingsMessage(this);
    missingSettingsMessage.setText(
        "Game folder path or DB path is invalid or missing. Please verify your "
        "settings.");
    missingSettingsMessage.exec();
    refreshButton->setDisabled(false);
    return;
  }

  modDataLoader.loadMods(mods, gameInstallPath / "Data");
  modDataLoader.loadMods(mods, gameInstallPath / "Mods");
  modDataLoader.loadMods(mods,
                         fs::path(settings.getWorkshopPath().toStdString()));

  dependencyDatabaseLoader.loadDependencyDatabase(mods);

  QStringList modNames;
  for (const auto &modManifest : mods.getMods()) {
    modNames.push_back(modManifest.getName());
  }

  QStringList activeModNames;
  for (const auto &activeModId : modsConfig.getActiveMods()) {
    if (mods.hasModWithPackageId(activeModId)) {
      activeModNames.push_back(mods.getModByPackageId(activeModId).getName());
    }
  }

  // TODO: Is it fine to hold only mod names in the views? This could pose an
  // issue in the unlikely case where two mods in the modlist collide on name.
  // However, creating a Qt model that can hold a combo of package ID and name
  // seems like a major hassle.
  enabledModsModel->setStringList(activeModNames);
  disabledModsModel->setStringList(modNames);
  refreshButton->setDisabled(false);
}

void MainWindow::onSortButtonClicked() {
  sortButton->setDisabled(true);
  QStringList activeModIds;
  QStringList sortedModNames;

  for (const auto &activeModName : enabledModsModel->stringList()) {
    activeModIds.push_back(mods.getModByName(activeModName).getPackageId());
  }

  DependencyGraph graph(activeModIds, modsConfig.getKnownExpansions(),
                        mods.getDependencies());

  try {
    for (const auto &packageId : graph.getSortedPackageIds()) {
      auto modName = mods.getModByPackageId(packageId).getName();
      sortedModNames.push_back(modName);
    }
    enabledModsModel->setStringList(sortedModNames);
  } catch (const CircularDependencies &circularDependencies) {
    CircularDependenciesDialog dialog(circularDependencies, mods, this);
    dialog.exec();
  }

  sortButton->setDisabled(false);
}

void MainWindow::onSaveButtonClicked() {
  QStringList activeModIds;

  for (const auto &activeModName : enabledModsModel->stringList()) {
    activeModIds.push_back(mods.getModByName(activeModName).getPackageId());
  }

  modsConfig.setActiveMods(activeModIds);
  modsConfigLoader.saveModsConfig(modsConfig);
}

void MainWindow::moveSelectedItems(QListView *sourceView,
                                   QListView *targetView) {
  auto sourceModel = sourceView->model();
  auto targetModel = targetView->model();
  for (const auto &selectedIndex :
       sourceView->selectionModel()->selectedIndexes()) {
    auto modName = sourceModel->data(selectedIndex);
    auto nextRow = targetModel->rowCount();
    targetModel->insertRow(nextRow);
    targetModel->setData(targetModel->index(nextRow, 0), modName);
    sourceModel->removeRow(selectedIndex.row());
  }

  targetView->scrollToBottom();
}

void MainWindow::moveAllItems(QListView *sourceView, QListView *targetView) {
  auto sourceModel = sourceView->model();
  auto targetModel = targetView->model();
  for (auto i = 0; i < sourceModel->rowCount(); i++) {
    auto rowIndex = sourceModel->index(i, 0);
    auto modName = sourceModel->data(rowIndex);

    auto nextRow = targetModel->rowCount();
    targetModel->insertRow(nextRow);
    targetModel->setData(targetModel->index(nextRow, 0), modName);
  }

  sourceModel->removeRows(0, sourceModel->rowCount());
}

void MainWindow::onModClickedInList(const QModelIndex &selected) {
  auto sourceModel = selected.model();
  auto modName = sourceModel->data(selected).toString();

  if (mods.hasModNamed(modName)) {
    headerTabs->setTabEnabled(modDetailsTabIndex, true);
    emit selectedModChanged(mods.getModByName(modName));
    headerTabs->setCurrentIndex(modDetailsTabIndex);
  }
}

}  // namespace qmodsorter
