#pragma once

#include <rune/rune.hpp>  // vector_map

#include <tactile-base/map_position.hpp>
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

using TileCache = rune::vector_map<MapPosition, TileID>;

}  // namespace Tactile