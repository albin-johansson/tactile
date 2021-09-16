#pragma once

#include <charconv>      // from_chars
#include <concepts>      // integral, floating_point
#include <string_view>   // string_view
#include <system_error>  // errc

#include "aliases/maybe.hpp"

namespace Tactile {

template <std::integral T>
[[nodiscard]] auto FromString(const std::string_view str, const int base = 10) -> Maybe<T>
{
  T value{};

  const auto [ptr, error] =
      std::from_chars(str.data(), str.data() + str.size(), value, base);
  if (error == std::errc{}) {
    return value;
  }
  else {
    return nothing;
  }
}

template <std::floating_point T>
[[nodiscard]] auto FromString(const std::string_view str) -> Maybe<T>
{
  T value{};

  const auto [ptr, error] = std::from_chars(str.data(), str.data() + str.size(), value);
  if (error == std::errc{}) {
    return value;
  }
  else {
    return nothing;
  }
}

}  // namespace Tactile
