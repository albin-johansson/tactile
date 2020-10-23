#pragma once

#include <QString>
#include <QWidget>
#include <optional>  // optional

namespace tactile::gui {

/**
 * @brief Spawns a dialog for selecting a tileset image and returns
 * the path of the selected image.
 *
 * @param parent the parent widget.
 *
 * @return the path of the image; `std::nullopt`if no file was selected.
 *
 * @since 0.1.0
 */
[[nodiscard]] auto open_tileset_image(QWidget* parent = nullptr)
    -> std::optional<QString>;

}  // namespace tactile::gui
