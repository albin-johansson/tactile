#pragma once

#include "tactile.hpp"
#include "tile_position.hpp"
#include "utils/vector_map.hpp"

namespace tactile {

using TileCache = vector_map<tile_position, tile_id>;

}  // namespace tactile