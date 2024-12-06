// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/common/serdes/tiles.hpp"

#include <bit>
#include <concepts>
#include <cstring>

#include "tactile/base/numeric/literals.hpp"
#include "tactile/base/platform/bits.hpp"

namespace tactile::common {
namespace {

// Update the documentation if the representation of TileID changes.
static_assert(std::same_as<TileID, std::int32_t>);

constexpr std::uint32_t kTiledFlippedHorizontallyBit = 1u << 31u;
constexpr std::uint32_t kTiledFlippedVerticallyBit = 1u << 30u;
constexpr std::uint32_t kTiledFlippedDiagonallyBit = 1u << 29u;
constexpr std::uint32_t kTiledRotatedHexagonal120Bit = 1u << 28u;

constexpr std::uint32_t kTiledTileFlippingMask =
    kTiledFlippedHorizontallyBit | kTiledFlippedVerticallyBit | kTiledFlippedDiagonallyBit |
    kTiledRotatedHexagonal120Bit;

}  // namespace

auto deserialize_tile_matrix(const ByteSpan byte_stream,
                             const Extent2D& extent,
                             const TileIdFormat tile_id_format) -> std::optional<TileMatrix>
{
  auto tile_matrix = make_tile_matrix(extent);

  const auto expected_byte_count = extent.rows * extent.cols * sizeof(TileID);
  const auto real_byte_count = byte_stream.size_bytes();

  if (expected_byte_count != real_byte_count) {
    return std::nullopt;
  }

  const auto tile_count = byte_stream.size_bytes() / sizeof(TileID);
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

auto serialize_tile_matrix(const TileMatrix& tile_matrix,
                           const std::span<std::uint8_t> output_buffer)
    -> std::expected<std::size_t, ErrorCode>
{
  ByteStream bytes {};

  const auto row_count = tile_matrix.size();
  if (row_count < 1_uz) {
    return std::unexpected {ErrorCode::kBadParam};
  }

  const auto col_count = tile_matrix.front().size();
  if (col_count < 1_uz) {
    return std::unexpected {ErrorCode::kBadParam};
  }

  const auto required_space = row_count * col_count * sizeof(TileID);
  if (output_buffer.size_bytes() < required_space) {
    return std::unexpected {ErrorCode::kBadParam};
  }

  auto byte_index = 0_uz;
  for (auto row = 0_uz; row < row_count; ++row) {
    for (auto col = 0_uz; col < col_count; ++col) {
      const auto tile_id = to_little_endian(tile_matrix[row][col]);
      each_byte(tile_id, [&](const std::uint8_t byte) {
        output_buffer[byte_index] = byte;
        ++byte_index;
      });
    }
  }

  return byte_index;
}

}  // namespace tactile::common
