#pragma once

#include <vector>  // vector

#include "common/tile_id.hpp"

namespace Tactile {

using TileRow = std::vector<TileID>;
using TileMatrix = std::vector<TileRow>;

}  // namespace Tactile
