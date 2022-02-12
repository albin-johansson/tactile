#pragma once

#include <vector>  // vector

#include "tactile.hpp"

namespace tactile {

using zlib_data = std::vector<uchar>;

/**
 * \brief Attempts to compress data using Zlib.
 *
 * \param data the source data that will be compressed.
 * \param bytes the size of the data in bytes.
 *
 * \return the compressed data; an empty optional is returned upon failure.
 */
[[nodiscard]] auto compress_with_zlib(const void* data, usize bytes) -> maybe<zlib_data>;

/**
 * \brief Restores data previously compressed with ZLib.
 *
 * \param data the compressed data to restore.
 * \param bytes the size of the data in bytes.
 *
 * \return the uncompressed data; an empty optional is returned upon failure.
 */
[[nodiscard]] auto decompress_with_zlib(const void* data, usize bytes) -> maybe<zlib_data>;

}  // namespace tactile
