// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <bit>       // endian, byteswap
#include <concepts>  // same_as
#include <cstring>   // memcpy
#include <utility>   // cmp_not_equal

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/io/byte_stream.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/matrix_extent.hpp"
#include "tactile/base/util/matrix_index.hpp"
#include "tactile/base/util/tile_matrix.hpp"

namespace tactile {

/**
 * Represents the different possible tile identifier representation modes.
 */
enum class TileIdFormat : uint8
{
  /** Native format, no special bits. */
  kTactile,

  /** The format used by Tiled, where upper bits in tile identifiers have special meanings. */
  kTiled,
};

// Update the documentation if the representation of TileID changes.
static_assert(std::same_as<TileID, int32>);

inline constexpr uint32 kTiledFlippedHorizontallyBit = 1u << 31u;
inline constexpr uint32 kTiledFlippedVerticallyBit = 1u << 30u;
inline constexpr uint32 kTiledFlippedDiagonallyBit = 1u << 29u;
inline constexpr uint32 kTiledRotatedHexagonal120Bit = 1u << 28u;

inline constexpr uint32 kTiledTileFlippingMask =
    kTiledFlippedHorizontallyBit | kTiledFlippedVerticallyBit | kTiledFlippedDiagonallyBit |
    kTiledRotatedHexagonal120Bit;

/**
 * Reconstructs a tile matrix from a byte stream.
 *
 * \param byte_stream    The tile matrix byte stream.
 * \param extent         The expected extent of the tile matrix.
 * \param tile_id_format The format used by the binary tile identifiers.
 *
 * \return
 * The parsed tile matrix if successful; an empty optional otherwise.
 */
constexpr auto parse_raw_tile_matrix(const ByteStream& byte_stream,
                                     const MatrixExtent& extent,
                                     const TileIdFormat tile_id_format) -> Optional<TileMatrix>
{
  auto tile_matrix = make_tile_matrix(extent);

  const auto expected_byte_count = extent.rows * extent.cols * sizeof(TileID);
  const auto real_byte_count = byte_stream.size();

  if (std::cmp_not_equal(expected_byte_count, real_byte_count)) {
    return kNone;
  }

  const auto tile_count = byte_stream.size() / sizeof(TileID);
  for (usize tile_index = 0; tile_index < tile_count; ++tile_index) {
    TileID tile_id {};

    const auto byte_index = tile_index * sizeof tile_id;
    std::memcpy(&tile_id, &byte_stream[byte_index], sizeof tile_id);

    // Tiles are stored using little endian byte ordering.
    if constexpr (std::endian::native == std::endian::big) {
      tile_id = std::byteswap(tile_id);
    }

    // Clear any flipping bits used by Tiled.
    if (tile_id_format == TileIdFormat::kTiled) {
      tile_id &= ~kTiledTileFlippingMask;
    }

    const auto [row, col] = make_matrix_index(saturate_cast<ssize>(tile_index), extent.cols);
    tile_matrix[static_cast<usize>(row)][static_cast<usize>(col)] = tile_id;
  }

  return tile_matrix;
}

}  // namespace tactile
