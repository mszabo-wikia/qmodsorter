#ifndef PATH_SELECTOR_PANEL_HPP
#define PATH_SELECTOR_PANEL_HPP

#include <QLineEdit>
#include <QWidget>

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
   * @param labelText label to be attached to this widget
   * @param defaultValue default path
   * @param parent Parent of this widget in the object tree (optional)
   */
  PathSelectorPanel(const QString &labelText, const QString &defaultValue,
                    QWidget *parent = nullptr);
  /**
   * @brief Set whether this panel should select directories or files.
   *
   */
  void setDirectorySelector(bool);
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

#endif
