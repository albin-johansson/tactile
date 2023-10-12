// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/tile_matrix_encoding.hpp"

#include <bit>       // endian, byteswap
#include <concepts>  // same_as
#include <cstring>   // memcpy

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/io/base64.hpp"
#include "tactile/core/io/compression/compression_error.hpp"
#include "tactile/core/platform/bits.hpp"

namespace tactile {

using namespace int_literals;

// Update the documentation if the representation of TileID changes.
static_assert(std::same_as<TileID, int32>);

auto tile_matrix_to_byte_stream(const TileMatrix& matrix) -> ByteStream
{
  TACTILE_ASSERT(matrix.size() >= 1_uz);
  TACTILE_ASSERT(matrix.at(0).size() >= 1_uz);

  const auto row_count = matrix.size();
  const auto col_count = matrix.at(0).size();

  ByteStream byte_stream;
  byte_stream.reserve(row_count * col_count * sizeof(TileID));

  for (usize row = 0; row < row_count; ++row) {
    for (usize col = 0; col < col_count; ++col) {
      // Tile identifiers are always stored as little endian values.
      const auto tile_id = to_little_endian(matrix[row][col]);
      each_byte(tile_id, [&](const uint8 byte) { byte_stream.push_back(byte); });
    }
  }

  return byte_stream;
}

auto tile_matrix_from_byte_stream(const ByteStream& byte_stream,
                                  const MatrixExtent& matrix_extent,
                                  const TileEncodingFormat encoding_format) -> TileMatrix
{
  auto tile_matrix = make_tile_matrix(matrix_extent);
  const auto tile_count = byte_stream.size() / sizeof(TileID);

  for (usize tile_index = 0; tile_index < tile_count; ++tile_index) {
    TileID tile_id {};

    const auto byte_index = tile_index * sizeof tile_id;
    std::memcpy(&tile_id, &byte_stream[byte_index], sizeof tile_id);

    // Correct the byte ordering if we're on a big endian system.
    if constexpr (std::endian::native == std::endian::big) {
      tile_id = std::byteswap(tile_id);
    }

    // Clear any flipping bits used by Tiled.
    if (encoding_format == TileEncodingFormat::kTiled) {
      tile_id &= ~kTiledTileFlippingMask;
    }

    const auto [row, col] = to_matrix_index(tile_index, matrix_extent.col_count);
    tile_matrix[row][col] = tile_id;
  }

  return tile_matrix;
}

auto base64_encode_tile_matrix(const TileMatrix& tiles,
                               const CompressionMode compression_mode) -> Result<String>
{
  const auto byte_stream = tile_matrix_to_byte_stream(tiles);

  switch (compression_mode) {
    case CompressionMode::kNone: return Base64::encode(byte_stream);

    case CompressionMode::kZlib: [[fallthrough]];
    case CompressionMode::kZstd:
      return compress_with(compression_mode, byte_stream)
          .and_then([](const ByteStream& compressed_bytes) -> Result<String> {
            return Base64::encode(compressed_bytes);
          });
  }

  return error(CompressionError::kInvalidMode);
}

auto base64_decode_tile_matrix(const StringView tile_data,
                               const MatrixExtent& matrix_extent,
                               const CompressionMode compression_mode,
                               const TileEncodingFormat encoding_format)
    -> Result<TileMatrix>
{
  const auto decoded_bytes = Base64::decode(tile_data);

  switch (compression_mode) {
    case CompressionMode::kNone:
      return tile_matrix_from_byte_stream(decoded_bytes, matrix_extent, encoding_format);

    case CompressionMode::kZlib: [[fallthrough]];
    case CompressionMode::kZstd:
      return decompress_with(compression_mode, decoded_bytes)
          .and_then([&](const ByteStream& decompressed_bytes) -> Result<TileMatrix> {
            return tile_matrix_from_byte_stream(decompressed_bytes,
                                                matrix_extent,
                                                encoding_format);
          });
  }

  return error(CompressionError::kInvalidMode);
}

}  // namespace tactile
