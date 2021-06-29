#pragma once

#include "core/map/map_position.hpp"

namespace Tactile {

struct TilesetSelection final
{
  MapPosition top_left;      ///< The top-left corner position.
  MapPosition bottom_right;  ///< The bottom-right corner position.
};

}  // namespace Tactile
