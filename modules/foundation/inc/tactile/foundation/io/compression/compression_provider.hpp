// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/byte_stream.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Interface for data compression (and decompression) providers.
 */
class ICompressionProvider {
 public:
  TACTILE_INTERFACE_CLASS(ICompressionProvider);

  /**
   * Attempts to compress a byte stream.
   *
   * \param data The data that will be compressed.
   *
   * \return
   *    A compressed byte stream; or an error code if an error occurred.
   */
  [[nodiscard]]
  virtual auto compress(ByteSpan data) const -> Result<ByteStream> = 0;

  /**
   * Attempts to decompress a compressed byte stream.
   *
   * \param data The data that will be decompressed.
   *
   * \return
   *    An uncompressed byte stream; or an error code if an error occurred.
   */
  [[nodiscard]]
  virtual auto decompress(ByteSpan data) const -> Result<ByteStream> = 0;
};

}  // namespace tactile