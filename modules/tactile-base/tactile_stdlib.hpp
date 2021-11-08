#ifndef TACTILE_TACTILE_STDLIB_HPP_
#define TACTILE_TACTILE_STDLIB_HPP_

#include <algorithm>     // replace
#include <charconv>      // from_chars
#include <concepts>      // integral, floating_point
#include <cstring>       // strlen
#include <exception>     // exception
#include <filesystem>    // path
#include <string>        // string, stof
#include <system_error>  // errc
#include <utility>       // pair
#include <vector>        // vector

#include "tactile_def.hpp"

namespace Tactile {

class TactileError final : public std::exception
{
 public:
  TactileError() noexcept = default;

  explicit TactileError(const CStr what) : mWhat{what ? what : "N/A"}
  {}

  [[nodiscard]] auto what() const noexcept -> CStr override
  {
    return mWhat;
  }

 private:
  CStr mWhat{"N/A"};
};

template <std::integral T>
[[nodiscard]] constexpr auto ToMatrixCoords(const T index, const T nColumns) noexcept
    -> std::pair<T, T>
{
  return {index / nColumns, index % nColumns};
}

[[nodiscard]] inline auto MakeTileRow(const usize nCols) -> TileRow
{
  TileRow row;
  row.reserve(nCols);
  row.assign(nCols, 0);
  return row;
}

[[nodiscard]] inline auto MakeTileMatrix(const usize nRows, const usize nCols)
    -> TileMatrix
{
  TileMatrix tiles;
  tiles.reserve(nRows);
  tiles.assign(nRows, MakeTileRow(nCols));
  return tiles;
}

template <std::integral T>
[[nodiscard]] auto FromString(const CStr str, const usize length, const int base = 10)
    -> Maybe<T>
{
  if (!str) {
    return nothing;
  }

  T value{};

  const auto* begin = str;
  const auto* end = str + length;
  const auto [ptr, err] = std::from_chars(begin, end, value, base);

  if (err == std::errc{}) {
    return value;
  }
  else {
    return nothing;
  }
}

template <std::integral T>
[[nodiscard]] auto FromString(const CStr str, const int base = 10) -> Maybe<T>
{
  if (!str) {
    return nothing;
  }
  else {
    return FromString<T>(str, std::strlen(str), base);
  }
}

template <std::floating_point T>
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

[[nodiscard]] inline auto ConvertToForwardSlashes(const std::filesystem::path& path)
    -> std::string
{
  /* Here we make sure that the file path is portable, by using forward slashes that
     can be understood by pretty much all operating systems that we care about. */
  auto str = path.string();
  std::replace(str.begin(), str.end(), '\\', '/');
  return str;
}

}  // namespace Tactile

#endif  // TACTILE_TACTILE_STDLIB_HPP_
