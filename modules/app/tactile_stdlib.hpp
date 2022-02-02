#pragma once

#include <algorithm>   // replace
#include <filesystem>  // path
#include <string>      // string
#include <utility>     // pair

#include "core/utils/sfinae.hpp"
#include "tactile_def.hpp"

namespace tactile {

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
[[nodiscard]] constexpr auto to_matrix_coords(const T index, const T nColumns)
    -> std::pair<T, T>
{
  return {index / nColumns, index % nColumns};
}

}  // namespace tactile
