// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <vector>  // vector

#include "tactile/base/id.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/matrix_extent.hpp"

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
constexpr auto make_tile_matrix(const MatrixExtent& extent) -> TileMatrix
{
  return {saturate_cast<usize>(extent.rows),
          TileRow(saturate_cast<usize>(extent.cols), kEmptyTile)};
}

}  // namespace tactile
