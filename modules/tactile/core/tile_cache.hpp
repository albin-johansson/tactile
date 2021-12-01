#pragma once

#include <tactile_def.hpp>

#include "core/map_position.hpp"
#include "core/utils/vector_map.hpp"

namespace Tactile {

using TileCache = VectorMap<MapPosition, TileID>;

}  // namespace Tactile