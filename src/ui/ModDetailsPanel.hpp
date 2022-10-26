#ifndef MOD_DETAILS_PANEL_HPP
#define MOD_DETAILS_PANEL_HPP

#include <ModManifest.hpp>
#include <QLabel>
#include <QWidget>

/**
 * @brief Widget that displays details (description etc.) for a single mod.
 */
class ModDetailsPanel : public QWidget {
  Q_OBJECT
 public:
  ModDetailsPanel();
 public slots:
  /**
   * @brief Called when the currently selected mod changes.
   *
   * @param selectedMod reference to the newly selected mod
   */
  void onSelectedModChanged(const ModManifest& selectedMod);

 private:
  QLabel* modImage;
  QLabel* modName;
  QLabel* packageId;
  QLabel* path;
  QLabel* description;
};

#endif
