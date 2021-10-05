#pragma once

#include <rune/rune.hpp>  // vector_map

#include "core/map_position.hpp"
#include "tile_id.hpp"

namespace Tactile {

using TileCache = rune::vector_map<MapPosition, TileID>;

}  // namespace Tactile