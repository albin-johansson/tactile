#pragma once

#include <charconv>      // from_chars
#include <concepts>      // integral, floating_point
#include <string>        // string, stof
#include <string_view>   // string_view
#include <system_error>  // errc

#include "tactile_std.hpp"

namespace Tactile {

template <std::integral T>
[[nodiscard]] auto FromString(const std::string_view str, const int base = 10) -> Maybe<T>
{
  T value{};

  const auto* begin = str.data();
  const auto* end = str.data() + str.size();
  const auto [ptr, err] = std::from_chars(begin, end, value, base);

  if (err == std::errc{}) {
    return value;
  }
  else {
    return nothing;
  }
}

template <std::floating_point T>
[[nodiscard]] auto FromString(const std::string_view str) -> Maybe<T>
{
  /* We only do this check in the floating-point overload, because it's the only version
     that isn't widely supported (at the time of writing, only MSVC provides it) */
#if __cpp_lib_to_chars >= 201611L
  T value{};

  const auto [ptr, err] = std::from_chars(str.data(), str.data() + str.size(), value);
  if (err == std::errc{}) {
    return value;
  }
  else {
    return nothing;
  }
#else
  try {
    return static_cast<T>(std::stof(std::string{str}));
  }
  catch (...) {
    return nothing;
  }
#endif  // __cpp_lib_to_chars >= 201611L
}

}  // namespace Tactile
