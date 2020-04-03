#pragma once
#include <QString>

#include "maybe.h"

class QWidget;

namespace tactile {

/**
 * Opens a file chooser dialog for selecting a tile sheet image and returns the
 * path of the selected file.
 *
 * @param parent the parent widget, defaults to null.
 * @return the path of the image, if a file was selected; nothing otherwise.
 * @since 0.1.0
 */
[[nodiscard]] Maybe<QString> open_tile_sheet_image(
    QWidget* parent = nullptr) noexcept;

}  // namespace tactile
