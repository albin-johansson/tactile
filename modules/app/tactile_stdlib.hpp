#pragma once

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

namespace tactile {

template <typename T>
using Integral = std::enable_if_t<std::is_integral_v<T>, int>;

template <typename T>
using Floating = std::enable_if_t<std::is_floating_point_v<T>, int>;

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
 * \see `FromString(c_str, int)`
 */
template <typename T, Integral<T> = 0>
[[nodiscard]] auto FromString(const c_str str, const usize length, const int base)
    -> maybe<T>
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
 * \see `FromString(c_str, usize, int)`
 */
template <typename T, Integral<T> = 0>
[[nodiscard]] auto FromString(const c_str str, const int base = 10) -> maybe<T>
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
[[nodiscard]] auto FromString(const c_str str) -> maybe<T>
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

}  // namespace tactile
