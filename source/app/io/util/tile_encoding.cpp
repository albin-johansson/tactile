#include "tile_encoding.hpp"

#include <bit>       // endian
#include <concepts>  // same_as
#include <span>      // span, as_bytes

#include <cppcodec/base64_default_rfc4648.hpp>

#include "core/utils/tiles.hpp"
#include "io/compression.hpp"
#include "misc/panic.hpp"

namespace tactile {
namespace {

/* Update documentation if the representation of TileID changes */
static_assert(std::same_as<TileID, int32>);

[[nodiscard]] auto _convert_tile_matrix_to_sequence(const TileMatrix& matrix,
                                                    const usize       rows,
                                                    const usize       columns)
    -> std::vector<TileID>
{
  std::vector<TileID> vec;
  vec.reserve(rows * columns);

  for (usize row = 0; row < rows; ++row) {
    for (usize col = 0; col < columns; ++col) {
      vec.push_back(matrix[row][col]);
    }
  }

  return vec;
}

[[nodiscard]] auto _decode_tiles(const std::vector<uchar>& data,
                                 const usize               rows,
                                 const usize               columns) -> TileMatrix
{
  auto matrix = make_tile_matrix(rows, columns);

  const auto count = data.size() / sizeof(int32);
  for (usize i = 0; i < count; ++i) {
    const auto index = i * 4;

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
  const auto sequence = _convert_tile_matrix_to_sequence(tiles, rows, columns);

  const std::span span {sequence};
  const auto      bytes = std::as_bytes(span);

  switch (compression) {
    case TileCompression::None: {
      return base64::encode(bytes);
    }
    case TileCompression::Zlib: {
      const auto compressed = compress_with_zlib(bytes.data(),  //
                                                 bytes.size(),
                                                 ZlibCompressionLevel::Default)
                                  .value();
      const auto compressedBytes = std::as_bytes(std::span {compressed});
      return base64::encode(compressedBytes);
    }
    default:
      throw TactileError {"Invalid tile compression!"};
  }
}

auto base64_decode_tiles(const std::string&    tiles,
                         const usize           rows,
                         const usize           columns,
                         const TileCompression compression) -> TileMatrix
{
  const auto decoded = base64::decode(tiles);

  switch (compression) {
    case TileCompression::None:
      return _decode_tiles(decoded, rows, columns);

    case TileCompression::Zlib: {
      const auto decompressed =
          decompress_with_zlib(decoded.data(), decoded.size()).value();
      return _decode_tiles(decompressed, rows, columns);
    }
    default:
      throw TactileError {"Invalid tile compression!"};
  }
}

}  // namespace tactile