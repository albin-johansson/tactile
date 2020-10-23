#include "open_tileset_image.hpp"

#include <QFileDialog>
#include <QStandardPaths>
#include <QTranslator>

namespace tactile::gui {

auto open_tileset_image(QWidget* parent) -> std::optional<QUrl>
{
  static QString cachedDirPath{""};

  QFileDialog dialog{parent};
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setViewMode(QFileDialog::Detail);

  // FIXME investigate why JPG doesn't seem to work when included
  dialog.setNameFilter(QTranslator::tr("Images (*.png *.jpg)"));
  dialog.setWindowTitle("Open tileset");

  if (cachedDirPath.isEmpty()) {
    using paths = QStandardPaths;
    dialog.setDirectory(paths::writableLocation(paths::PicturesLocation));
  } else {
    dialog.setDirectory(cachedDirPath);
  }

  const auto success = dialog.exec();
  cachedDirPath = dialog.directoryUrl().toString();

  if (success) {
    return dialog.selectedUrls().first();
  } else {
    return std::nullopt;
  }
}

}  // namespace tactile::gui
