#ifndef TACTILE_TACTILE_STDLIB_HPP_
#define TACTILE_TACTILE_STDLIB_HPP_

#include <algorithm>     // replace, min
#include <charconv>      // from_chars
#include <cstring>       // strlen
#include <exception>     // exception
#include <filesystem>    // path
#include <string>        // string, stof
#include <system_error>  // errc
#include <type_traits>   // enable_if_t, is_integral_v, is_floating_point_v
#include <utility>       // pair
#include <vector>        // vector

#include "tactile_def.hpp"

namespace Tactile {

template <typename T>
using Integral = std::enable_if_t<std::is_integral_v<T>, int>;

template <typename T>
using Floating = std::enable_if_t<std::is_floating_point_v<T>, int>;

class TactileError : public std::exception {
 public:
  TactileError() noexcept = default;

  explicit TactileError(const CStr what) : mWhat{what ? what : "N/A"} {}

  [[nodiscard]] auto what() const noexcept -> CStr override { return mWhat; }

 private:
  CStr mWhat{"N/A"};
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
template <typename T, Integral<T> = 0>
[[nodiscard]] constexpr auto ToMatrixCoords(const T index, const T nColumns)
    -> std::pair<T, T>
{
  return {index / nColumns, index % nColumns};
}

/**
 * \brief Creates a row of tiles.
 *
 * \param nCols the number of columns.
 *
 * \return a row filled with empty tile identifiers.
 */
[[nodiscard]] inline auto MakeTileRow(const usize nCols) -> TileRow
{
  TileRow row;
  row.reserve(nCols);
  row.assign(nCols, 0);
  return row;
}

/**
 * \brief Creates a tile matrix with the specified dimensions.
 *
 * \param nRows the number or rows.
 * \param nCols the number of columns.
 *
 * \return a matrix with empty tile identifiers.
 */
[[nodiscard]] inline auto MakeTileMatrix(const usize nRows, const usize nCols)
    -> TileMatrix
{
  TileMatrix tiles;
  tiles.reserve(nRows);
  tiles.assign(nRows, MakeTileRow(nCols));
  return tiles;
}

/**
 * \brief Converts a string into an integer.
 *
 * \details This function is particularly useful for converting substrings into integers.
 *
 * \param str the string that will be converted to an integer, can safely be null.
 * \param length the length of the section of the string that will be processed.
 * \param base the base used by the parsed integer.
 *
 * \return an integer using the specified base; `nothing` if something goes wrong.
 *
 * \see `FromString(CStr, int)`
 */
template <typename T, Integral<T> = 0>
[[nodiscard]] auto FromString(const CStr str, const usize length, const int base)
    -> Maybe<T>
{
  if (!str) {
    return nothing;
  }

  T value{};

  const auto* begin = str;
  const auto* end = str + (std::min)(length, std::strlen(str));
  const auto [ptr, err] = std::from_chars(begin, end, value, base);

  if (err == std::errc{}) {
    return value;
  }
  else {
    return nothing;
  }
}

/**
 * \brief Converts a full string into an integer.
 *
 * \param str the string that will be converted to an integer, can safely be null.
 * \param base the base used by the parsed integer.
 *
 * \return an integer using the specified base; `nothing` if something goes wrong.
 *
 * \see `FromString(CStr, usize, int)`
 */
template <typename T, Integral<T> = 0>
[[nodiscard]] auto FromString(const CStr str, const int base = 10) -> Maybe<T>
{
  if (!str) {
    return nothing;
  }
  else {
    return FromString<T>(str, std::strlen(str), base);
  }
}

/**
 * \brief Converts a string into a floating-point value.
 *
 * \param str the string that will be converted to a float, can safely be null.
 *
 * \return the corresponding floating-point value; `nothing` if something goes wrong.
 */
template <typename T, Floating<T> = 0>
[[nodiscard]] auto FromString(const CStr str) -> Maybe<T>
{
  if (!str) {
    return nothing;
  }

  /* We only do this check in the floating-point overload, because it's the only version
     that isn't widely supported (at the time of writing, only MSVC provides it) */
#if __cpp_lib_to_chars >= 201611L
  T value{};

  const auto [ptr, err] = std::from_chars(str, str + std::strlen(str), value);
  if (err == std::errc{}) {
    return value;
  }
  else {
    return nothing;
  }
#else
  try {
    return static_cast<T>(std::stof(str));
  }
  catch (...) {
    return nothing;
  }
#endif  // __cpp_lib_to_chars >= 201611L
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
[[nodiscard]] inline auto ConvertToForwardSlashes(const std::filesystem::path& path)
    -> std::string
{
  auto str = path.string();
  std::replace(str.begin(), str.end(), '\\', '/');
  return str;
}

}  // namespace Tactile

#endif  // TACTILE_TACTILE_STDLIB_HPP_
