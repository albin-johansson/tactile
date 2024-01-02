// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/byte_stream.hpp"
#include "tactile/foundation/io/compression/compression_kind.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Compresses the given data with the specified compression strategy.
 *
 * \param mode The compression strategy to use.
 * \param data The source data.
 *
 * \return
 *    The compressed data; or an error code if something went wrong.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto compress_with(CompressionMode mode, ByteSpan data)
    -> Result<ByteStream>;

/**
 * Decompresses the given data with the specified compression strategy.
 *
 * \param mode The compression strategy used to decompress the data.
 * \param data The compressed data.
 *
 * \return
 *    The decompressed data; or an error code if something went wrong.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto decompress_with(CompressionMode mode, ByteSpan data)
    -> Result<ByteStream>;

}  // namespace tactile
