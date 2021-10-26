#pragma once

#include <tactile-base/map_position.hpp>
#include <tactile-base/tactile_std.hpp>

#include <rune/containers/vector_map.hpp>  // vector_map

namespace Tactile {

using TileCache = rune::vector_map<MapPosition, TileID>;

}  // namespace Tactile