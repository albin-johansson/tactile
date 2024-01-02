// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/span.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/byte_stream.hpp"
#include "tactile/foundation/io/compression/compression_kind.hpp"
#include "tactile/foundation/misc/tile_matrix.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

inline constexpr uint32 kTiledFlippedHorizontallyBit = 1u << 31u;
inline constexpr uint32 kTiledFlippedVerticallyBit = 1u << 30u;
inline constexpr uint32 kTiledFlippedDiagonallyBit = 1u << 29u;
inline constexpr uint32 kTiledRotatedHexagonal120Bit = 1u << 28u;

inline constexpr uint32 kTiledTileFlippingMask =
    kTiledFlippedHorizontallyBit | kTiledFlippedVerticallyBit |
    kTiledFlippedDiagonallyBit | kTiledRotatedHexagonal120Bit;

/**
 * Represents different tile data encoding formats.
 */
enum class TileEncodingFormat : uint8 {
  kTactile,  ///< The native Tactile format.
  kTiled,    ///< The format used by Tiled, most notably includes tile flipping bits.
};

/**
 * Converts a tile matrix into a byte stream.
 *
 * \details
 *    The tile identifiers are always converted to little endian integers
 *    and output row-by-row.
 *
 * \pre The tile matrix dimensions must be at least be 1x1.
 *
 * \param matrix The source tile matrix.
 *
 * \return
 *    A byte stream.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto tile_matrix_to_byte_stream(const TileMatrix& matrix)
    -> ByteStream;

/**
 * Restores a tile matrix from a byte stream.
 *
 * \param byte_stream     The source byte stream.
 * \param matrix_extent   The dimensions of the tile matrix.
 * \param encoding_format The tile encoding format used to interpret the data.
 *
 * \return
 *    A tile matrix.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto tile_matrix_from_byte_stream(
    const ByteStream& byte_stream,
    const MatrixExtent& matrix_extent,
    TileEncodingFormat encoding_format) -> TileMatrix;

/**
 * Encodes the given tile matrix using Base64.
 *
 * \details
 *    The given tile matrix is first converted to a byte stream. The byte stream is
 *    then compressed if compression is requested. The resulting byte stream is then
 *    encoded using Base64.
 *
 * \param tiles            The source tile data.
 * \param compression_mode The compression mode to use.
 *
 * \return
 *    The encoded tile data; or an error code if something failed.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto base64_encode_tile_matrix(const TileMatrix& tiles,
                                                      CompressionMode compression_mode)
    -> Result<String>;

/**
 * Decodes the given Base64 encoded tile data.
 *
 * \details
 *    This function is essentially the inverse of `base64_encode_tile_matrix`.
 *
 * \param tile_data        The Base64 encoded tile data.
 * \param matrix_extent    The dimensions of the tile matrix.
 * \param compression_mode The compression mode used by the tile data.
 * \param encoding_format  The tile encoding format used to interpret the data.
 *
 * \return
 *    The decoded tile matrix; or an error code if something failed.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto base64_decode_tile_matrix(StringView tile_data,
                                                      const MatrixExtent& matrix_extent,
                                                      CompressionMode compression_mode,
                                                      TileEncodingFormat encoding_format)
    -> Result<TileMatrix>;

}  // namespace tactile
