/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <concepts>  // integral

#include "core/common/ints.hpp"
#include "core/components/layers.hpp"

namespace tactile {

template <typename T>
struct MatrixCoords final
{
  T row{};
  T col{};
};

/**
 * \brief Converts a matrix index to the corresponding row/column pair.
 *
 * \details This function is useful when parsing tile streams from save files, where you
 * are likely to want to convert the indices to more intuitive matrix coordinates.
 *
 * \param index the index of the cell to determine the coordinates of.
 * \param nColumns the total number of columns in the matrix.
 *
 * \return a pair encoding the matrix coordinates as (row, column).
 */
template <std::integral T>
[[nodiscard]] constexpr auto to_matrix_coords(const T index, const T nColumns) noexcept
    -> MatrixCoords<T>
{
  return {index / nColumns, index % nColumns};
}

[[nodiscard]] auto make_tile_row(usize nCols) -> TileRow;

/**
 * \brief Creates a tile matrix with the specified dimensions.
 *
 * \param nRows the number or rows.
 * \param nCols the number of columns.
 *
 * \return a matrix with empty tile identifiers.
 */
[[nodiscard]] auto make_tile_matrix(usize nRows, usize nCols) -> TileMatrix;

}  // namespace tactile
