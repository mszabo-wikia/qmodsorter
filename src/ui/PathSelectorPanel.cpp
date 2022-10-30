#include "PathSelectorPanel.hpp"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>

PathSelectorPanel::PathSelectorPanel(const QString &defaultValue)
    : pathDisplayField(new QLineEdit()) {
  pathDisplayField->setReadOnly(true);
  pathDisplayField->setText(defaultValue);
  auto pathSelectorButton = new QPushButton("Select");

  auto inputLayout = new QHBoxLayout();
  inputLayout->addWidget(pathDisplayField);
  inputLayout->addWidget(pathSelectorButton);

  setLayout(inputLayout);

  connect(pathSelectorButton, &QPushButton::clicked, this,
          &PathSelectorPanel::onSelectPathButtonClicked);
}

void PathSelectorPanel::setDirectorySelector(
    bool isDirectorySelector) noexcept {
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
