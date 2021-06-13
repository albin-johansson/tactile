#include "tileset_image_file_dialog.hpp"

#include "standard_paths.hpp"

namespace tactile {

TilesetImageFileDialog::TilesetImageFileDialog(QWidget* parent)
    : QFileDialog{parent}
{
  setObjectName(QStringLiteral(u"TilesetImageFileDialog"));
  setFileMode(ExistingFile);
  setViewMode(Detail);
  setNameFilter(tr("Images (*.png *.jpg)"));
  setWindowTitle(tr("Open tileset"));
  setDirectory(GetPicturesPath());
}

}  // namespace tactile
