#include "open_tileset_image.hpp"

#include "tileset_image_file_dialog.hpp"

namespace tactile {

auto OpenTilesetImage(QWidget* parent) -> Maybe<QString>
{
  static QString cachedPath;

  TilesetImageFileDialog dialog{parent};

  if (!cachedPath.isEmpty())
  {
    dialog.setDirectory(cachedPath);
  }

  const auto success = dialog.exec();
  cachedPath = dialog.directory().path();

  if (success)
  {
    return dialog.selectedFiles().first();
  }
  else
  {
    return nothing;
  }
}

}  // namespace tactile
