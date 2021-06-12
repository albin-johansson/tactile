#pragma once

#include "map_position.hpp"

namespace tactile::core {

/**
 * \struct TilesetSelection
 *
 * \brief Represents the selection of tiles in a tileset.
 *
 * \details The two positions represent a rectangular selection.
 *
 * \since 0.1.0
 *
 * \headerfile tileset.hpp
 */
struct TilesetSelection final
{
  MapPosition topLeft;      ///< The top-left corner position.
  MapPosition bottomRight;  ///< The bottom-right corner position.
};

}  // namespace tactile::core
