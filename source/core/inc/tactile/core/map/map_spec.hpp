// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/layer/tile_orientation.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/matrix_extent.hpp"

namespace tactile {

/**
 * Provides the information necessary to create a map.
 */
struct MapSpec final
{
  /** The tile orientation mode to use. */
  TileOrientation orientation;

  /** The initial extent of the map. */
  MatrixExtent extent;

  /** The logical size of tiles in the map. */
  Int2 tile_size;
};

/**
 * Indicates whether a map specification is valid.
 *
 * \param spec The specification to check.
 *
 * \return
 * True if the specification is valid; false otherwise.
 */
[[nodiscard]]
auto is_valid(const MapSpec& spec) -> bool;

}  // namespace tactile
