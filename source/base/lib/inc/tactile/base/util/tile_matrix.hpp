// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <vector>  // vector

#include "tactile/base/id.hpp"
#include "tactile/base/numeric/extent_2d.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

using TileRow = std::vector<TileID>;
using TileMatrix = std::vector<TileRow>;

/**
 * Creates a tile matrix of a given size with empty tile identifiers.
 *
 * \param extent The initial tile matrix extent.
 *
 * \return
 * A tile matrix.
 */
[[nodiscard]]
constexpr auto make_tile_matrix(const Extent2D& extent) -> TileMatrix
{
  return {extent.rows, TileRow(extent.cols, kEmptyTile)};
}

}  // namespace tactile
