#include "base64_tiles.hpp"

#include <bit>       // endian
#include <concepts>  // same_as
#include <span>      // span, as_bytes

#include <cppcodec/base64_rfc4648.hpp>

#include "core/util/bits.hpp"
#include "core/util/functional.hpp"
#include "core/util/tiles.hpp"
#include "io/compression.hpp"
#include "misc/panic.hpp"

using Base64 = cppcodec::base64_rfc4648;

namespace tactile::io {
namespace {

// Update documentation if the representation of TileID changes
static_assert(std::same_as<TileID, int32>);

[[nodiscard]] auto convert_tile_matrix_to_sequence(const TileMatrix& matrix,
                                                   const usize rows,
                                                   const usize columns) -> ByteStream
{
  ByteStream seq;
  seq.reserve(rows * columns * sizeof(TileID));

  invoke_mn(rows, columns, [&](const usize row, const usize col) {
    // Always store tile identifiers as little endian values
    const auto tile_id = as_little_endian(matrix[row][col]);

    seq.push_back(nth_byte(tile_id, 0));
    seq.push_back(nth_byte(tile_id, 1));
    seq.push_back(nth_byte(tile_id, 2));
    seq.push_back(nth_byte(tile_id, 3));
  });

  return seq;
}

[[nodiscard]] auto restore_tiles(const std::vector<uchar>& data,
                                 const usize rows,
                                 const usize columns) -> TileMatrix
{
  auto matrix = make_tile_matrix(rows, columns);

  const auto count = data.size() / sizeof(int32);
  for (usize i = 0; i < count; ++i) {
    const auto index = i * sizeof(int32);

    const auto a = static_cast<int32>(data[index]);
    const auto b = static_cast<int32>(data[index + 1]);
    const auto c = static_cast<int32>(data[index + 2]);
    const auto d = static_cast<int32>(data[index + 3]);

    const TileID tile = std::endian::native == std::endian::little
                            ? (d << 24) | (c << 16) | (b << 8) | a
                            : (a << 24) | (b << 16) | (c << 8) | d;

    const auto [row, col] = to_matrix_coords(i, columns);
    matrix[row][col] = tile;
  }

  return matrix;
}

}  // namespace

auto base64_encode_tiles(const TileMatrix& tiles,
                         const usize rows,
                         const usize columns,
                         const TileCompression compression) -> std::string
{
  const auto sequence = convert_tile_matrix_to_sequence(tiles, rows, columns);

  switch (compression) {
    case TileCompression::None: {
      return Base64::encode(sequence);
    }
    case TileCompression::Zlib: {
      const auto compressed = zlib_compress(sequence).value();
      return Base64::encode(compressed);
    }
    case TileCompression::Zstd: {
      const auto compressed = zstd_compress(sequence).value();
      return Base64::encode(compressed);
    }
    default:
      throw TactileError {"Invalid compression strategy!"};
  }
}

auto base64_decode_tiles(const std::string& tiles,
                         const usize rows,
                         const usize columns,
                         const TileCompression compression) -> TileMatrix
{
  const auto decoded_bytes = Base64::decode(tiles.data(), tiles.size());

  switch (compression) {
    case TileCompression::None:
      return restore_tiles(decoded_bytes, rows, columns);

    case TileCompression::Zlib: {
      const auto decompressed = zlib_decompress(decoded_bytes).value();
      return restore_tiles(decompressed, rows, columns);
    }
    case TileCompression::Zstd: {
      const auto decompressed = zstd_decompress(decoded_bytes).value();
      return restore_tiles(decompressed, rows, columns);
    }
    default:
      throw TactileError {"Invalid compression strategy!"};
  }
}

}  // namespace tactile::io