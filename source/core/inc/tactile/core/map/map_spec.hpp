// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/map/map_orientation.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/matrix_extent.hpp"

namespace tactile {

/**
 * Provides the information necessary to create a map.
 */
struct MapSpec final
{
  /** The orientation mode to use. */
  MapOrientation orientation;

  /** The initial extent of the map. */
  MatrixExtent extent;

  /** The logical size of tiles in the map. */
  Int2 tile_size;
};

}  // namespace tactile
