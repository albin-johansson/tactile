// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <ranges>  // continuous_range

#include "tactile/core/container/span.hpp"
#include "tactile/core/container/vector.hpp"
#include "tactile/core/functional/result.hpp"
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
 * \interface ICompressionProvider
 * \brief Interface for data compression (and decompression) providers.
 */
class ICompressionProvider {
 public:
  TACTILE_INTERFACE_CLASS(ICompressionProvider);

  /**
   * \brief Attempts to compress a byte stream.
   *
   * \param data the data that will be compressed.
   *
   * \return a compressed byte stream; or an error code if an error occurred.
   */
  [[nodiscard]]
  virtual auto compress(ByteSpan data) const -> Result<ByteStream> = 0;

  /**
   * \brief Attempts to decompress a compressed byte stream.
   *
   * \param data the data that will be decompressed.
   *
   * \return an uncompressed byte stream; or an error code if an error occurred.
   */
  [[nodiscard]]
  virtual auto decompress(ByteSpan data) const -> Result<ByteStream> = 0;
};

/**
 * \brief Creates a byte span from a container.
 *
 * \param container a contiguous container, such as an array.
 *
 * \return a byte span.
 */
template <std::ranges::contiguous_range T>
[[nodiscard]] auto make_byte_span(const T& container) -> ByteSpan
{
  return ByteSpan {static_cast<const uint8*>(static_cast<const void*>(container.data())),
                   container.size() * sizeof(T)};
}

}  // namespace tactile