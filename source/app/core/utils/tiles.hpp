#pragma once

#include "core/components/layer.hpp"
#include "sfinae.hpp"
#include "tactile_def.hpp"

namespace tactile {

template <typename T>
struct matrix_coords final
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
template <typename T, is_integral<T> = 0>
[[nodiscard]] constexpr auto to_matrix_coords(const T index, const T nColumns) noexcept
    -> matrix_coords<T>
{
  return {index / nColumns, index % nColumns};
}

[[nodiscard]] auto make_tile_row(usize nCols) -> tile_row;

/**
 * \brief Creates a tile matrix with the specified dimensions.
 *
 * \param nRows the number or rows.
 * \param nCols the number of columns.
 *
 * \return a matrix with empty tile identifiers.
 */
[[nodiscard]] auto make_tile_matrix(usize nRows, usize nCols) -> tile_matrix;

}  // namespace tactile
