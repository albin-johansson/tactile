#pragma once

#include <QString>  // QString

#include "maybe.hpp"

class QWidget;

namespace tactile {

/**
 * \brief Spawns a dialog for selecting a tileset image and returns
 * the path of the selected image.
 *
 * \param parent the parent widget.
 *
 * \return the path of the image; `nothing`if no file was selected.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto OpenTilesetImage(QWidget* parent = nullptr)
    -> Maybe<QString>;

}  // namespace tactile
