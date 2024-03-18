// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "base64_tiles.hpp"

#include <bit>       // byteswap, endian
#include <concepts>  // same_as
#include <cstring>   // memcpy

#include <cppcodec/base64_rfc4648.hpp>

#include "common/util/functional.hpp"
#include "core/tile/tile_matrix.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/io/compress/zlib_compression_provider.hpp"
#include "tactile/core/io/compress/zstd_compression_provider.hpp"
#include "tactile/core/platform/bits.hpp"

using Base64 = cppcodec::base64_rfc4648;

namespace tactile {
namespace {

// Update documentation if the representation of TileID changes
static_assert(std::same_as<TileID, int32>);

[[nodiscard]] auto convert_tile_matrix_to_sequence(const TileMatrix& matrix,
                                                   const TileExtent extent) -> ByteStream
{
  ByteStream seq;
  seq.reserve(extent.rows * extent.cols * sizeof(TileID));

  invoke_mn(extent.rows, extent.cols, [&](const usize row, const usize col) {
    // Always store tile identifiers as little endian values
    const auto tile_id = to_little_endian(matrix[row][col]);
    each_byte(tile_id, [&](const uint8 byte) { seq.push_back(byte); });
  });

  return seq;
}

[[nodiscard]] auto restore_tiles(const auto& data, const TileExtent extent) -> TileMatrix
{
  auto matrix = make_tile_matrix(extent);

  const auto count = data.size() / sizeof(int32);
  for (usize i = 0; i < count; ++i) {
    const auto index = i * sizeof(int32);

    TileID tile {};
    std::memcpy(&tile, &data[index], sizeof(TileID));

    if constexpr (std::endian::native == std::endian::big) {
      tile = std::byteswap(tile);
    }

    const auto [row, col] = to_matrix_coords(i, extent.cols);
    matrix[row][col] = tile;
  }

  return matrix;
}

[[nodiscard]] auto encode_bytes(const ByteStream& stream) -> String
{
  return Base64::encode(stream.data(), stream.size());
}

}  // namespace

auto base64_encode_tiles(const TileMatrix& tiles,
                         const TileExtent extent,
                         const TileCompression compression) -> String
{
  const auto byte_stream = convert_tile_matrix_to_sequence(tiles, extent);

  switch (compression) {
    case TileCompression::None: {
      return encode_bytes(byte_stream);
    }
    case TileCompression::Zlib: {
      const auto compressed_bytes =
          ZlibCompressionProvider {}.compress(byte_stream).value();
      return encode_bytes(compressed_bytes);
    }
    case TileCompression::Zstd: {
      const auto compressed_bytes =
          ZstdCompressionProvider {}.compress(byte_stream).value();
      return encode_bytes(compressed_bytes);
    }
    default: throw Exception {"Invalid compression strategy"};
  }
}

auto base64_decode_tiles(StringView tiles,
                         const TileExtent extent,
                         const TileCompression compression) -> TileMatrix
{
  const auto decoded_bytes = Base64::decode(tiles.data(), tiles.size());

  switch (compression) {
    case TileCompression::None: return restore_tiles(decoded_bytes, extent);

    case TileCompression::Zlib: {
      const auto decompressed_bytes =
          ZlibCompressionProvider {}.decompress(decoded_bytes).value();
      return restore_tiles(decompressed_bytes, extent);
    }
    case TileCompression::Zstd: {
      const auto decompressed_bytes =
          ZstdCompressionProvider {}.decompress(decoded_bytes).value();
      return restore_tiles(decompressed_bytes, extent);
    }
    default: throw Exception {"Invalid compression strategy"};
  }
}

}  // namespace tactile
