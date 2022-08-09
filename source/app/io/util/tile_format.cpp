#include "tile_format.hpp"

#include <bit>       // endian
#include <concepts>  // same_as
#include <span>      // span, as_bytes

#include <cppcodec/base64_default_rfc4648.hpp>

#include "core/util/functional.hpp"
#include "core/util/tiles.hpp"
#include "io/compression.hpp"
#include "misc/panic.hpp"

namespace tactile::io {
namespace {

// Update documentation if the representation of TileID changes
static_assert(std::same_as<TileID, int32>);

[[nodiscard]] auto convert_tile_matrix_to_sequence(const TileMatrix& matrix,
                                                   const usize       rows,
                                                   const usize       columns) -> TileRow
{
  TileRow seq;
  seq.reserve(rows * columns);

  invoke_mn(rows, columns, [&](const usize row, const usize col) {
    seq.push_back(matrix[row][col]);
  });

  return seq;
}

[[nodiscard]] auto restore_tiles(const std::vector<uchar>& data,
                                 const usize               rows,
                                 const usize               columns) -> TileMatrix
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
                            ? (d << 3) | (c << 2) | (b << 1) | a
                            : (a << 3) | (b << 2) | (c << 1) | d;

    const auto [row, col] = to_matrix_coords(i, columns);
    matrix[row][col] = tile;
  }

  return matrix;
}

}  // namespace

auto base64_encode_tiles(const TileMatrix&     tiles,
                         const usize           rows,
                         const usize           columns,
                         const TileCompression compression) -> std::string
{
  const auto sequence = convert_tile_matrix_to_sequence(tiles, rows, columns);
  const auto bytes = std::as_bytes(std::span {sequence});

  switch (compression) {
    case TileCompression::None: {
      return base64::encode(bytes);
    }
    case TileCompression::Zlib: {
      const auto compressed = zlib_compress(bytes.data(), bytes.size_bytes()).value();
      return base64::encode(std::as_bytes(std::span {compressed}));
    }
    default:
      throw TactileError {"Invalid compression strategy!"};
  }
}

auto base64_decode_tiles(const std::string&    tiles,
                         const usize           rows,
                         const usize           columns,
                         const TileCompression compression) -> TileMatrix
{
  const auto bytes = std::as_bytes(std::span {tiles});
  const auto decoded_bytes = base64::decode(bytes);

  switch (compression) {
    case TileCompression::None:
      return restore_tiles(decoded_bytes, rows, columns);

    case TileCompression::Zlib: {
      const auto decompressed = zlib_decompress(decoded_bytes).value();
      return restore_tiles(decompressed, rows, columns);
    }
    default:
      throw TactileError {"Invalid compression strategy!"};
  }
}

}  // namespace tactile::io