// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include "core/common/identifiers.hpp"
#include "core/layer/tile_layer.hpp"

namespace tactile::test {

void set_all_tiles(TileLayer& layer, TileID tile);

void verify_all_tiles_matches(const TileLayer& layer, TileID tile);

[[nodiscard]] auto add_tile_layer(Map& map) -> TileLayer&;

}  // namespace tactile::test
