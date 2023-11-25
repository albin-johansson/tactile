// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/functional/result.hpp"
#include "tactile/core/io/byte_stream.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \interface ICompressionProvider
 * \brief Interface for data compression (and decompression) providers.
 */
class TACTILE_CORE_API ICompressionProvider {
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

}  // namespace tactile