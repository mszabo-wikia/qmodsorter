#include "CircularDependenciesDialog.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

namespace qmodsorter {

CircularDependenciesDialog::CircularDependenciesDialog(
    const CircularDependencies& circularDependencies,
    const ModManifestList& modManifestList, QWidget* parent)
    : QDialog(parent) {
  setWindowTitle("Circular dependencies found");

  auto description = new QLabel(
      "Unable to automatically sort the active mod list because it contains "
      "the following dependency loops:");

  auto mainLayout = new QGridLayout();
  mainLayout->addWidget(description, 0, 0, 1, 2);

  const auto& circles = circularDependencies.getCircles();

  for (auto i = 0; i < circles.size(); i++) {
    auto separator = new QFrame();

    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);

    auto index =
        new QLabel(QString::fromStdString("(" + std::to_string(i + 1) + ")"));
    QString circleAsString;

    for (auto j = 0; j < circles[i].size(); j++) {
      circleAsString +=
          modManifestList.getModByPackageId(circles[i][j]).getName();
      if (j < circles[i].size() - 1) {
        circleAsString += " \u2192 ";  // right arrow
      }
    }
    auto loop = new QLabel(circleAsString.toHtmlEscaped());
    loop->setContentsMargins(0, 9, 0, 9);

    mainLayout->addWidget(separator, i + 1, 0, 1, 2);
    mainLayout->addWidget(index, i + 2, 0);
    mainLayout->addWidget(loop, i + 2, 1);
  }

  auto okButton = new QPushButton("OK");
  okButton->setDefault(true);

  connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

  mainLayout->addWidget(okButton, circles.size() + 2, 0, 1, 2, Qt::AlignCenter);

  setLayout(mainLayout);
}

}  // namespace qmodsorter
