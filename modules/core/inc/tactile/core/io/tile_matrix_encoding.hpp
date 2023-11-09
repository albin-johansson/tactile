// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/container/span.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/functional/result.hpp"
#include "tactile/core/io/byte_stream.hpp"
#include "tactile/core/io/compression/compression.hpp"
#include "tactile/core/misc/tile_matrix.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

inline constexpr uint32 kTiledFlippedHorizontallyBit = 1u << 31u;
inline constexpr uint32 kTiledFlippedVerticallyBit = 1u << 30u;
inline constexpr uint32 kTiledFlippedDiagonallyBit = 1u << 29u;
inline constexpr uint32 kTiledRotatedHexagonal120Bit = 1u << 28u;

inline constexpr uint32 kTiledTileFlippingMask =
    kTiledFlippedHorizontallyBit | kTiledFlippedVerticallyBit |
    kTiledFlippedDiagonallyBit | kTiledRotatedHexagonal120Bit;

/**
 * \brief Represents different tile data encoding formats.
 */
enum class TileEncodingFormat : uint8 {
  kTactile,  ///< The native Tactile format.
  kTiled,    ///< The format used by Tiled, most notably includes tile flipping bits.
};

/**
 * \brief Converts a tile matrix into a byte stream.
 *
 * \details The tile identifiers are always converted to little endian integers and output
 *          row-by-row.
 *
 * \pre The tile matrix dimensions must be at least be 1x1.
 *
 * \param matrix the source tile matrix.
 *
 * \return a byte stream.
 */
[[nodiscard]]
TACTILE_CORE_API auto tile_matrix_to_byte_stream(const TileMatrix& matrix) -> ByteStream;

/**
 * \brief Restores a tile matrix from a byte stream.
 *
 * \param byte_stream     the source byte stream.
 * \param matrix_extent   the dimensions of the tile matrix.
 * \param encoding_format the tile encoding format used to interpret the data.
 *
 * \return a tile matrix.
 */
[[nodiscard]]
TACTILE_CORE_API auto tile_matrix_from_byte_stream(const ByteStream& byte_stream,
                                                   const MatrixExtent& matrix_extent,
                                                   TileEncodingFormat encoding_format)
    -> TileMatrix;

/**
 * \brief Encodes the given tile matrix using Base64.
 *
 * \details The given tile matrix is first converted to a byte stream. The byte stream is
 *          then compressed if compression is requested. The resulting byte stream is then
 *          encoded using Base64.
 *
 * \param tiles            the source tile data.
 * \param compression_mode the compression mode to use.
 *
 * \return the encoded tile data; or an error code if something failed.
 */
[[nodiscard]]
TACTILE_CORE_API auto base64_encode_tile_matrix(const TileMatrix& tiles,
                                                CompressionMode compression_mode)
    -> Result<String>;

/**
 * \brief Decodes the given Base64 encoded tile data.
 *
 * \details This function is essentially the inverse of `base64_encode_tile_matrix`.
 *
 * \param tile_data        the Base64 encoded tile data.
 * \param matrix_extent    the dimensions of the tile matrix.
 * \param compression_mode the compression mode used by the tile data.
 * \param encoding_format  the tile encoding format used to interpret the data.
 *
 * \return the decoded tile matrix; or an error code if something failed.
 */
[[nodiscard]]
TACTILE_CORE_API auto base64_decode_tile_matrix(StringView tile_data,
                                                const MatrixExtent& matrix_extent,
                                                CompressionMode compression_mode,
                                                TileEncodingFormat encoding_format)
    -> Result<TileMatrix>;

}  // namespace tactile
