#pragma once

#include <algorithm>   // replace
#include <filesystem>  // path
#include <string>      // string
#include <utility>     // pair

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
template <typename T>
[[nodiscard]] constexpr auto to_matrix_coords(const T index, const T nColumns)
    -> std::pair<T, T>
{
  return {index / nColumns, index % nColumns};
}

/**
 * \brief Converts a path to a string that is guaranteed to use forward slashes.
 *
 * \details This function is useful when saving paths to files in a portable way. Since
 * all relevant operating systems understand forward slashes, even if some operating
 * systems prefer backslashes (such as Windows).
 *
 * \param path the file path that will be converted.
 *
 * \return a version of the path with forward slashes instead of backslashes.
 */
[[nodiscard]] inline auto convert_to_forward_slashes(const std::filesystem::path& path)
    -> std::string
{
  auto str = path.string();
  std::replace(str.begin(), str.end(), '\\', '/');
  return str;
}

}  // namespace tactile
