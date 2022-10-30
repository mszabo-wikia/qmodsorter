#ifndef PATH_SELECTOR_PANEL_HPP
#define PATH_SELECTOR_PANEL_HPP

#include <QLineEdit>
#include <QWidget>

namespace qmodsorter {

/**
 * @brief Widget that allows selecting a filesystem path via a dialog and
 * displays it inline.
 */
class PathSelectorPanel : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Construct a new widget instance with the given label and default
   * path.
   *
   * @param defaultValue default path
   */
  PathSelectorPanel(const QString &defaultValue);
  /**
   * @brief Set whether this panel should select directories or files.
   *
   */
  void setDirectorySelector(bool) noexcept;
  QString getPathName() const;

 private slots:
  void onSelectPathButtonClicked();
 signals:
  /**
   * @brief Emitted when the user selects a path using the selection dialog.
   *
   * @param path user-selected path
   */
  void pathSelected(const QString &path);

 private:
  QLineEdit *pathDisplayField;
  bool isDirectorySelector;
};

}  // namespace qmodsorter

#endif
