#include "tileset_image_file_dialog.hpp"

#include <QStandardPaths>

namespace tactile::gui {

tileset_image_file_dialog::tileset_image_file_dialog(QWidget* parent)
    : QFileDialog{parent}
{
  setFileMode(ExistingFile);
  setViewMode(Detail);
  setNameFilter(tr("Images (*.png *.jpg)"));
  setWindowTitle(tr("Open tileset"));
  setDirectory(
      QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
}

}  // namespace tactile::gui
