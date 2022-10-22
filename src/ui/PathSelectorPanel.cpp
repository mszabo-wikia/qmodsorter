#include "PathSelectorPanel.hpp"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

PathSelectorPanel::PathSelectorPanel(const QString &labelText,
                                     const QString &defaultValue,
                                     QWidget *parent)
    : QWidget(parent), pathDisplayField(new QLineEdit()) {
  auto label = new QLabel(labelText);
  pathDisplayField->setReadOnly(true);
  pathDisplayField->setText(defaultValue);
  auto pathSelectorButton = new QPushButton("Select");

  auto inputLayout = new QHBoxLayout();
  inputLayout->addWidget(pathDisplayField);
  inputLayout->addWidget(pathSelectorButton);

  auto mainLayout = new QVBoxLayout();
  mainLayout->addWidget(label);
  mainLayout->addLayout(inputLayout);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  setLayout(mainLayout);

  connect(pathSelectorButton, &QPushButton::clicked, this,
          &PathSelectorPanel::onSelectPathButtonClicked);
}

void PathSelectorPanel::setDirectorySelector(bool isDirectorySelector) {
  this->isDirectorySelector = isDirectorySelector;
}

QString PathSelectorPanel::getPathName() const {
  return pathDisplayField->text();
}

void PathSelectorPanel::onSelectPathButtonClicked() {
  auto pathName = isDirectorySelector
                      ? QFileDialog::getExistingDirectory(this, "Select")
                      : QFileDialog::getOpenFileName(this, "Select");
  if (!pathName.isEmpty()) {
    pathDisplayField->setText(pathName);
    emit pathSelected(pathName);
  }
}
