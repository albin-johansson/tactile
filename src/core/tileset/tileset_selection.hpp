#pragma once

#include "position.hpp"

namespace tactile::core {

/**
 * \struct tileset_selection
 *
 * \brief Represents the selection of tiles in a tileset.
 *
 * \details The two positions represent a rectangular selection.
 *
 * \since 0.1.0
 *
 * \headerfile tileset.hpp
 */
struct tileset_selection final
{
  position topLeft;      ///< The top-left corner position.
  position bottomRight;  ///< The bottom-right corner position.
};

}  // namespace tactile::core
