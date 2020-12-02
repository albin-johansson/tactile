#include "open_tileset_image.hpp"

#include "tileset_image_file_dialog.hpp"

namespace tactile::gui {

auto open_tileset_image(QWidget* parent) -> maybe<QString>
{
  static QString cachedPath;

  tileset_image_file_dialog dialog{parent};

  if (!cachedPath.isEmpty()) {
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
