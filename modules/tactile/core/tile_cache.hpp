#pragma once

#include <tactile_def.hpp>

#include "core/map_position.hpp"
#include "core/utils/vector_map.hpp"

namespace tactile {

using TileCache = VectorMap<MapPosition, TileID>;

}  // namespace tactile