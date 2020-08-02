#include "open_tileset_image.hpp"

#include <QFileDialog>
#include <QStandardPaths>
#include <QTranslator>

namespace tactile {

Maybe<QUrl> open_tileset_image(QWidget* parent) noexcept
{
  static QString cachedDirPath{""};

  QFileDialog dialog{parent};
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setViewMode(QFileDialog::Detail);

  // TODO investigate why JPG doesn't seem to work when included
  dialog.setNameFilter(QTranslator::tr("Images (*.png)"));
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
    return dialog.selectedUrls().first();
  } else {
    return nothing;
  }
}

}  // namespace tactile
