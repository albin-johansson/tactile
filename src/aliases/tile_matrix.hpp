#pragma once

#include <vector>  // vector

#include "aliases/tile_id.hpp"

namespace Tactile {

using TileRow = std::vector<tile_id>;
using TileMatrix = std::vector<TileRow>;

}  // namespace Tactile
