// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // integral

#include "core/tile/tile_extent.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"

namespace tactile {

using TileRow = Vector<TileID>;
using TileMatrix = Vector<TileRow>;

template <typename T>
struct MatrixCoords final {
  T row {};
  T col {};
};

/// Converts a matrix index to the corresponding row/column pair.
///
/// This function is useful when parsing tile streams from save files, where you are
/// likely to want to convert the indices to more intuitive matrix coordinates.
///
/// \param index the index of the cell to determine the coordinates of.
/// \param nColumns the total number of columns in the matrix.
///
/// \return a pair encoding the matrix coordinates as (row, column).
template <std::integral T>
[[nodiscard]] constexpr auto to_matrix_coords(const T index, const T n_columns) noexcept
    -> MatrixCoords<T>
{
  return {index / n_columns, index % n_columns};
}

[[nodiscard]] auto make_tile_row(usize column_count) -> TileRow;

/// Creates a tile matrix of empty tile identifiers.
[[nodiscard]] auto make_tile_matrix(TileExtent extent) -> TileMatrix;

}  // namespace tactile
