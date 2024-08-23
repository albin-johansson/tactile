// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <charconv>      // from_chars
#include <optional>      // optional
#include <system_error>  // errc

#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Attempts to convert a string to an integer.
 *
 * \tparam T An integral type.
 *
 * \param str  The source string.
 * \param base The numerical base to use.
 *
 * \return
 * An integer if successful; an empty optional otherwise.
 */
template <std::integral T>
[[nodiscard]] constexpr auto parse(StringView str,
                                   const int base = 10) noexcept -> std::optional<T>
{
  const auto* const begin = str.data();
  const auto* const end = begin + str.size();

  T value {};
  const auto [ptr, err] = std::from_chars(begin, end, value, base);

  if (err == std::errc {} && ptr == end) {
    return value;
  }

  return std::nullopt;
}

}  // namespace tactile
