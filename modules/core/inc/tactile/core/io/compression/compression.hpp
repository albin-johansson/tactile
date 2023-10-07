// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/functional/result.hpp"
#include "tactile/core/io/byte_stream.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Represents different compression modes.
 */
enum class CompressionMode : uint8 {
  kNone,  ///< Use no compression.
  kZlib,  ///< Use Zlib compression.
  kZstd,  ///< Use Zstd compression.
};

/**
 * \brief Compresses the given data with the specified compression strategy.
 *
 * \param mode the compression strategy to use.
 * \param data the source data.
 *
 * \return the compressed data; or an error code if something went wrong.
 */
[[nodiscard]]
TACTILE_CORE_API auto compress_with(CompressionMode mode, ByteSpan data)
    -> Result<ByteStream>;

/**
 * \brief Decompresses the given data with the specified compression strategy.
 *
 * \param mode the compression strategy used to decompress the data.
 * \param data the compressed data.
 *
 * \return the decompressed data; or an error code if something went wrong.
 */
[[nodiscard]]
TACTILE_CORE_API auto decompress_with(CompressionMode mode, ByteSpan data)
    -> Result<ByteStream>;

}  // namespace tactile
