#ifndef CIRCULAR_DEPENDENCIES_DIALOG_HPP
#define CIRCULAR_DEPENDENCIES_DIALOG_HPP

#include <CircularDependencies.hpp>
#include <ModManifestList.hpp>
#include <QDialog>

/**
 * @brief Dialog to render a basic breakdown of circular dependencies found
 * while sorting.
 */
class CircularDependenciesDialog : public QDialog {
  Q_OBJECT

 public:
  CircularDependenciesDialog(const CircularDependencies& circularDependencies,
                             const ModManifestList& modManifestList,
                             QWidget* parent = nullptr);
};

#endif
