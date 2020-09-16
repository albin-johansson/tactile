#pragma once

#include <array>         // array
#include <charconv>      // to_chars
#include <optional>      // optional, nullopt
#include <string>        // string
#include <system_error>  // errc

#include "algorithm.hpp"  // arithmetic

namespace tactile {

/**
 * @brief Returns a string representation of an arithmetic value.
 *
 * @tparam T the type of the value that will be converted, must be arithmetic.
 * @tparam bufferSize the size of the stack buffer used, must be big enough
 * to store the characters of the string representation of the value.
 *
 * @param value the value that will be converted.
 *
 * @return a string representation of the supplied value; `std::nullopt` if
 * something goes wrong.
 *
 * @since 0.1.0
 */
template <arithmetic T, std::size_t bufferSize = 10>
[[nodiscard]] auto to_string(T value) -> std::optional<std::string>
{
  std::array<char, bufferSize> buffer{};
  auto first = buffer.data();
  if (const auto [ptr, err] =
          std::to_chars(first, first + buffer.size(), value);
      err == std::errc{}) {
    return std::string{buffer.data(), ptr};
  } else {
    return std::nullopt;
  }
}

}  // namespace tactile
