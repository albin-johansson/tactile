/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "base64_tiles.hpp"

#include <bit>          // endian
#include <concepts>     // same_as
#include <cstring>      // memcpy
#include <string_view>  // string_view

#include <folly/Bits.h>
#include <folly/base64.h>

#include "core/util/functional.hpp"
#include "core/util/numeric.hpp"
#include "core/util/string.hpp"
#include "core/util/tiles.hpp"
#include "io/compression.hpp"
#include "misc/panic.hpp"

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
    const auto tile_id = folly::Endian::little(matrix[row][col]);
    each_byte(tile_id, [&](const uint8 byte) { seq.push_back(byte); });
  });

  return seq;
}

[[nodiscard]] auto restore_tiles(const auto& data, const usize rows, const usize columns)
    -> TileMatrix
{
  auto matrix = make_tile_matrix(rows, columns);

  const auto count = data.size() / sizeof(int32);
  for (usize i = 0; i < count; ++i) {
    const auto index = i * sizeof(int32);

    TileID tile {};
    std::memcpy(&tile, &data[index], sizeof(TileID));

    if constexpr (std::endian::native == std::endian::big) {
      tile = folly::Endian::swap(tile);
    }

    const auto [row, col] = to_matrix_coords(i, columns);
    matrix[row][col] = tile;
  }

  return matrix;
}

[[nodiscard]] auto encode(const ByteStream& stream) -> String
{
  const auto* data = static_cast<const char*>(static_cast<const void*>(stream.data()));
  std::string_view view {data, stream.size()};
  return from_std(folly::base64Encode(view));
}

}  // namespace

auto base64_encode_tiles(const TileMatrix& tiles,
                         const usize rows,
                         const usize columns,
                         const TileCompression compression) -> String
{
  const auto sequence = convert_tile_matrix_to_sequence(tiles, rows, columns);

  switch (compression) {
    case TileCompression::None: {
      return encode(sequence);
    }
    case TileCompression::Zlib: {
      const auto compressed = zlib_compress(sequence).value();
      return encode(compressed);
    }
    case TileCompression::Zstd: {
      const auto compressed = zstd_compress(sequence).value();
      return encode(compressed);
    }
    default:
      throw TactileError {"Invalid compression strategy!"};
  }
}

auto base64_decode_tiles(StringView tiles,
                         const usize rows,
                         const usize columns,
                         const TileCompression compression) -> TileMatrix
{
  const auto decoded = folly::base64Decode(to_std_view(tiles));

  switch (compression) {
    case TileCompression::None:
      return restore_tiles(decoded, rows, columns);

    case TileCompression::Zlib: {
      const auto decompressed = zlib_decompress(decoded.data(), decoded.size()).value();
      return restore_tiles(decompressed, rows, columns);
    }
    case TileCompression::Zstd: {
      const auto decompressed = zstd_decompress(decoded.data(), decoded.size()).value();
      return restore_tiles(decompressed, rows, columns);
    }
    default:
      throw TactileError {"Invalid compression strategy!"};
  }
}

}  // namespace tactile::io