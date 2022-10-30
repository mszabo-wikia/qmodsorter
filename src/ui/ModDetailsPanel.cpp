#include "ModDetailsPanel.hpp"

#include <QFile>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QScrollArea>

namespace qmodsorter {

ModDetailsPanel::ModDetailsPanel()
    : QWidget(),
      modImage(new QLabel()),
      modName(new QLabel()),
      packageId(new QLabel),
      path(new QLabel()),
      description(new QLabel()) {
  modImage->setTextFormat(Qt::PlainText);
  modName->setTextFormat(Qt::PlainText);
  packageId->setTextFormat(Qt::PlainText);

  path->setTextFormat(Qt::RichText);
  path->setOpenExternalLinks(true);

  description->setTextFormat(Qt::PlainText);
  description->setWordWrap(true);
  description->setMargin(9);

  auto descriptionScrollArea = new QScrollArea();
  descriptionScrollArea->setWidget(description);
  descriptionScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  descriptionScrollArea->setWidgetResizable(true);
  descriptionScrollArea->setAlignment(Qt::AlignTop | Qt::AlignLeft);

  auto modNameLabel = new QLabel("Name:");
  auto packageIdLabel = new QLabel("Package ID:");
  auto pathLabel = new QLabel("Path:");

  auto detailsGridLayout = new QGridLayout();
  detailsGridLayout->addWidget(modNameLabel, 0, 0);
  detailsGridLayout->addWidget(modName, 0, 1);

  detailsGridLayout->addWidget(packageIdLabel, 0, 2);
  detailsGridLayout->addWidget(packageId, 0, 3);

  detailsGridLayout->addWidget(pathLabel, 1, 0);
  detailsGridLayout->addWidget(path, 1, 1);

  detailsGridLayout->addWidget(descriptionScrollArea, 2, 0, 1, 4);

  auto mainLayout = new QHBoxLayout();

  mainLayout->addLayout(detailsGridLayout);
  mainLayout->addSpacing(18);
  mainLayout->addWidget(modImage);
  mainLayout->setDirection(QHBoxLayout::RightToLeft);

  setLayout(mainLayout);
}

void ModDetailsPanel::onSelectedModChanged(const ModManifest &selectedMod) {
  auto modFolderPath = selectedMod.getFolder();
  auto modImagePath = modFolderPath / "About" / "Preview.png";

  // Case fold the mod image name as needed on case-sensitive file systems
  if (!fs::exists(modImagePath)) {
    modImagePath = modFolderPath / "About" / "preview.png";
  }

  QString modFolder(modFolderPath.c_str());
  QFile modImageFile(modImagePath);
  QPixmap modImagePix;

  if (modImageFile.open(QFile::ReadOnly) &&
      modImagePix.loadFromData(modImageFile.readAll())) {
    auto scaledHeight = std::min(modImagePix.height(), 128);
    auto scaledPix =
        modImagePix.scaledToHeight(scaledHeight, Qt::SmoothTransformation);
    modImage->setPixmap(scaledPix);
  } else {
    modImage->clear();
  }

  auto pathLink = "<a href=\"file://" + modFolder.toHtmlEscaped() + "\">" +
                  modFolder.toHtmlEscaped() + "</a>";

  modName->setText(selectedMod.getName());
  packageId->setText(selectedMod.getPackageId());
  path->setText(pathLink);
  description->setText(selectedMod.getDescription());
}

}  // namespace qmodsorter
