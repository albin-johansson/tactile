#pragma once

#include <optional>  // optional
#include <vector>    // vector

#include "core/common/ints.hpp"

namespace tactile {

using ZlibData = std::vector<uchar>;

/// Represents different compression levels passed on to Zlib.
enum class ZlibCompressionLevel
{
  Default,          ///< Uses Z_DEFAULT_COMPRESSION.
  BestCompression,  ///< Uses Z_BEST_COMPRESSION.
  BestSpeed         ///< Uses Z_BEST_SPEED.
};

/**
 * Attempts to compress data using Zlib.
 *
 * \param data the source data that will be compressed.
 * \param bytes the size of the data in bytes.
 * \param level the compression strategy used.
 *
 * \return the compressed data; an empty optional is returned upon failure.
 */
[[nodiscard]] auto compress_with_zlib(const void*          data,
                                      usize                bytes,
                                      ZlibCompressionLevel level)
    -> std::optional<ZlibData>;

/**
 * Restores data previously compressed with Zlib.
 *
 * \param data the compressed data to restore.
 * \param bytes the size of the data in bytes.
 *
 * \return the uncompressed data; an empty optional is returned upon failure.
 */
[[nodiscard]] auto decompress_with_zlib(const void* data, usize bytes)
    -> std::optional<ZlibData>;

}  // namespace tactile
