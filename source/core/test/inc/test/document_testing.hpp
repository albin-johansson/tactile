// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/map/map_spec.hpp"
#include "tactile/core/tile/tileset_spec.hpp"

namespace tactile::core {

inline constexpr MapSpec kOrthogonalMapSpec {
  .orientation = TileOrientation::kOrthogonal,
  .extent = Extent2D {10, 10},
  .tile_size = Int2 {50, 50},
};

inline const TilesetSpec kDummyTilesetSpec {
  .tile_size = Int2 {16, 16},
  .texture =
      CTexture {
        .raw_handle = nullptr,
        .id = TextureID {1},
        .size = Int2 {96, 64},
        .path = "assets/images/dummy.png",
      },
};

}  // namespace tactile::core
