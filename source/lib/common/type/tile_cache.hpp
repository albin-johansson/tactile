// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "common/type/vector_map.hpp"
#include "core/tile/tile_pos.hpp"
#include "tactile/base/id.hpp"

namespace tactile {

using TileCache = VectorMap<TilePos, TileID>;

}  // namespace tactile
