#pragma once

#include <charconv>      // from_chars
#include <string_view>   // string_view
#include <system_error>  // errc

#include "aliases/maybe.hpp"

namespace Tactile {

template <typename T = int>
[[nodiscard]] auto FromString(const std::string_view str, const int base = 10)
    -> Maybe<T>
{
  T value{};

  const auto [ptr, error] =
      std::from_chars(str.data(), str.data() + str.size(), value, base);
  if (error != std::errc::invalid_argument &&
      error != std::errc::result_out_of_range)
  {
    return value;
  }
  else
  {
    return nothing;
  }
}

}  // namespace Tactile
