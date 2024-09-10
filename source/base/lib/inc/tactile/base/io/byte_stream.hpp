// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t
#include <ranges>   // contiguous_range
#include <span>     // span
#include <vector>   // vector

#include "tactile/base/prelude.hpp"

namespace tactile {

/** A contiguous sequence of arbitrary bytes. */
using ByteSpan = std::span<const std::uint8_t>;

/** A contiguous stream of arbitrary bytes. */
using ByteStream = std::vector<std::uint8_t>;

/**
 * Creates a byte span from a container.
 *
 * \tparam T Any contiguous container type.
 *
 * \param container A contiguous container.
 *
 * \return
 * A byte span.
 */
template <std::ranges::contiguous_range T>
[[nodiscard]] constexpr auto make_byte_span(const T& container) -> ByteSpan
{
  return {reinterpret_cast<const std::uint8_t*>(container.data()),
          container.size() * sizeof(typename T::value_type)};
}

}  // namespace tactile
