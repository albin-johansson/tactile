#include "open_tileset_image.hpp"

#include <QFileDialog>
#include <QStandardPaths>
#include <QTranslator>

namespace tactile::gui {

auto open_tileset_image(QWidget* parent) -> maybe<QString>
{
  static QString cachedPath;

  QFileDialog dialog{parent};
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setViewMode(QFileDialog::Detail);

  dialog.setNameFilter(QTranslator::tr("Images (*.png *.jpg)"));
  dialog.setWindowTitle(QTranslator::tr("Open tileset"));

  if (cachedPath.isEmpty()) {
    dialog.setDirectory(
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
  } else {
    dialog.setDirectory(cachedPath);
  }

  const auto success = dialog.exec();
  cachedPath = dialog.directory().path();

  if (success) {
    return dialog.selectedFiles().first();
  } else {
    return std::nullopt;
  }
}

}  // namespace tactile::gui
