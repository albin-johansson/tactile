#pragma once

#include <tactile_def.hpp>

#include <rune/containers/vector_map.hpp>  // vector_map

#include "core/map_position.hpp"

namespace Tactile {

using TileCache = rune::vector_map<MapPosition, TileID>;

}  // namespace Tactile