#ifndef MOD_NAME_LIST_MODEL_HPP
#define MOD_NAME_LIST_MODEL_HPP

#include <QStringListModel>

/**
 * @brief Qt model holding a set of mod names.
 */
class ModNameListModel : public QStringListModel {
 public:
  ModNameListModel(QObject *parent = nullptr) : QStringListModel(parent) {}
  Qt::ItemFlags flags(const QModelIndex &index) const override {
    auto defaultFlags = QStringListModel::flags(index);

    // Prevent items from being overridden due to a drop operation
    if (index.isValid()) {
      return defaultFlags & ~(Qt::ItemIsEditable | Qt::ItemIsDropEnabled);
    }

    return defaultFlags;
  }
};

#endif
