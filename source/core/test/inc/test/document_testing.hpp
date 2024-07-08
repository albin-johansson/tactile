// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/map/map_spec.hpp"

namespace tactile::test {

inline constexpr MapSpec kOrthogonalMapSpec {
  .orientation = TileOrientation::kOrthogonal,
  .extent = MatrixExtent {10, 10},
  .tile_size = Int2 {50, 50},
};

}  // namespace tactile::test
