// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/io/byte_stream.hpp"

namespace tactile {

/** Interface for data compression providers. */
class ICompressionProvider {
 public:
  TACTILE_INTERFACE_CLASS(ICompressionProvider);

  /**
   * Attempts to compress a byte stream.
   *
   * \param input_data The data that will be compressed.
   *
   * \return
   *    A compressed byte stream if successful; an error code otherwise.
   */
  [[nodiscard]]
  virtual auto compress(ByteSpan input_data) const -> Result<ByteStream> = 0;

  /**
   * Attempts to decompress a compressed byte stream.
   *
   * \param input_data The data that will be decompressed.
   *
   * \return
   *    An uncompressed byte stream if successful; an error code otherwise.
   */
  [[nodiscard]]
  virtual auto decompress(ByteSpan input_data) const -> Result<ByteStream> = 0;
};

}  // namespace tactile
