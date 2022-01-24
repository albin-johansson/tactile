#pragma once

#include <tactile_def.hpp>

#include "core/tile_position.hpp"
#include "core/utils/vector_map.hpp"

namespace tactile {

using TileCache = vector_map<tile_position, TileID>;

}  // namespace tactile