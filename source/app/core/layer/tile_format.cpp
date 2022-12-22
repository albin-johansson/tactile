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

#include "tile_format.hpp"

#include <zlib.h>

#include "debug/panic.hpp"

namespace tactile {

// See https://github.com/facebook/zstd/issues/3133 for Zstd compression levels

void TileFormat::set_encoding(const TileEncoding encoding)
{
  mEncoding = encoding;

  if (mEncoding == TileEncoding::Plain) {
    mCompression = TileCompression::None;
  }
}

void TileFormat::set_compression(const TileCompression compression)
{
  if (can_use_compression_strategy(compression)) {
    mCompression = compression;
  }
  else {
    throw TactileError {"Invalid encoding/compression combination!"};
  }
}

void TileFormat::set_zlib_compression_level(const int level)
{
  if (is_valid_zlib_compression_level(level)) {
    mZlibCompressionLevel = level;
  }
  else {
    throw TactileError {"Invalid Zlib compression level!"};
  }
}

void TileFormat::set_zstd_compression_level(const int level)
{
  if (is_valid_zstd_compression_level(level)) {
    mZstdCompressionLevel = level;
  }
  else {
    throw TactileError {"Invalid zstd compression level!"};
  }
}

auto TileFormat::encoding() const -> TileEncoding
{
  return mEncoding;
}

auto TileFormat::compression() const -> TileCompression
{
  return mCompression;
}

auto TileFormat::zlib_compression_level() const -> int
{
  return mZlibCompressionLevel;
}

auto TileFormat::zstd_compression_level() const -> int
{
  return mZstdCompressionLevel;
}

auto TileFormat::supports_any_compression() const -> bool
{
  return mEncoding != TileEncoding::Plain;
}

auto TileFormat::can_use_compression_strategy(const TileCompression compression) const
    -> bool
{
  return compression == TileCompression::None || mEncoding != TileEncoding::Plain;
}

auto TileFormat::is_valid_zlib_compression_level(const int level) -> bool
{
  return level == Z_DEFAULT_COMPRESSION ||
         (level >= min_zlib_compression_level() && level <= max_zlib_compression_level());
}

auto TileFormat::is_valid_zstd_compression_level(const int level) -> bool
{
  return level >= min_zstd_compression_level() && level <= max_zstd_compression_level();
}

auto TileFormat::min_zlib_compression_level() -> int
{
  return Z_BEST_SPEED;
}

auto TileFormat::max_zlib_compression_level() -> int
{
  return Z_BEST_COMPRESSION;
}

auto TileFormat::min_zstd_compression_level() -> int
{
  return 1;
}

auto TileFormat::max_zstd_compression_level() -> int
{
  return 19;
}

}  // namespace tactile