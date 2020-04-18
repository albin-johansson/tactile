#include "open_tileset_image.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QTranslator>

namespace tactile {

Maybe<QString> open_tileset_image(QWidget* parent) noexcept
{
  static QString cachedDirPath{""};

  QFileDialog dialog{parent};
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setViewMode(QFileDialog::Detail);
  dialog.setNameFilter(QTranslator::tr("Images (*.png *.jpg)"));
  dialog.setWindowTitle("Open tileset");

  if (cachedDirPath.isEmpty()) {
    using Paths = QStandardPaths;
    dialog.setDirectory(Paths::writableLocation(Paths::PicturesLocation));
  } else {
    dialog.setDirectory(cachedDirPath);
  }

  const auto success = dialog.exec();
  cachedDirPath = dialog.directoryUrl().toString();

  if (success) {
    return dialog.selectedFiles().first();  // TODO support multiple selections?
  } else {
    return nothing;
  }
}

}  // namespace tactile
