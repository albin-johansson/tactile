// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <bit>       // endian, byteswap
#include <concepts>  // same_as
#include <cstddef>   // size_t
#include <cstdint>   // uint8_t, int32_t, uint32_t
#include <cstring>   // memcpy
#include <optional>  // optional
#include <vector>    // vector

#include "tactile/base/io/byte_stream.hpp"
#include "tactile/base/platform/bits.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/tile_matrix.hpp"

namespace tactile {

/**
 * Represents the different possible tile identifier representation modes.
 */
enum class TileIdFormat : std::uint8_t
{
  /** Native format, no special bits. */
  kTactile,

  /** The format used by Tiled, where upper bits in tile identifiers have special meanings. */
  kTiled,
};

// Update the documentation if the representation of TileID changes.
static_assert(std::same_as<TileID, std::int32_t>);

inline constexpr std::uint32_t kTiledFlippedHorizontallyBit = 1u << 31u;
inline constexpr std::uint32_t kTiledFlippedVerticallyBit = 1u << 30u;
inline constexpr std::uint32_t kTiledFlippedDiagonallyBit = 1u << 29u;
inline constexpr std::uint32_t kTiledRotatedHexagonal120Bit = 1u << 28u;

inline constexpr std::uint32_t kTiledTileFlippingMask =
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
                                     const Extent2D& extent,
                                     const TileIdFormat tile_id_format)
    -> std::optional<TileMatrix>
{
  auto tile_matrix = make_tile_matrix(extent);

  const auto expected_byte_count = extent.rows * extent.cols * sizeof(TileID);
  const auto real_byte_count = byte_stream.size();

  if (expected_byte_count != real_byte_count) {
    return std::nullopt;
  }

  const auto tile_count = byte_stream.size() / sizeof(TileID);
  for (std::size_t tile_index = 0; tile_index < tile_count; ++tile_index) {
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

    const auto index = Index2D::from_1d(tile_index, extent.cols);
    tile_matrix[index.y][index.x] = tile_id;
  }

  return tile_matrix;
}

[[nodiscard]]
inline auto to_byte_stream(const TileMatrix& tile_matrix) -> ByteStream
{
  ByteStream bytes {};

  const auto row_count = tile_matrix.size();
  if (row_count < 1) {
    return bytes;
  }

  const auto col_count = tile_matrix.front().size();
  if (col_count < 1) {
    return bytes;
  }

  bytes.reserve(row_count * col_count * sizeof(TileID));

  for (std::size_t row = 0; row < row_count; ++row) {
    for (std::size_t col = 0; col < col_count; ++col) {
      const auto tile_id = to_little_endian(tile_matrix[row][col]);
      each_byte(tile_id, [&](const std::uint8_t byte) { bytes.push_back(byte); });
    }
  }

  return bytes;
}

}  // namespace tactile
