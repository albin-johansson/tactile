// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <ranges>  // continuous_range

#include "tactile/core/container/span.hpp"
#include "tactile/core/container/vector.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Represents a read-only view into a byte stream.
 */
using ByteSpan = Span<const uint8>;

/**
 * \brief Represents an arbitrary stream of bytes.
 */
using ByteStream = Vector<uint8>;

/**
 * \brief Creates a byte span from a container.
 *
 * \param container a contiguous container, such as an array.
 *
 * \return a byte span.
 */
template <std::ranges::contiguous_range T>
[[nodiscard]] constexpr auto make_byte_span(const T& container) -> ByteSpan
{
  return ByteSpan {static_cast<const uint8*>(static_cast<const void*>(container.data())),
                   container.size() * sizeof(typename T::value_type)};
}

}  // namespace tactile