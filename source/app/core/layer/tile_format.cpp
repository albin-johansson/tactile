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

#include "misc/panic.hpp"

namespace tactile {

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

void TileFormat::set_endianness(const std::endian endian)
{
  mEndianness = endian;
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

auto TileFormat::encoding() const -> TileEncoding
{
  return mEncoding;
}

auto TileFormat::compression() const -> TileCompression
{
  return mCompression;
}

auto TileFormat::endianness() const -> std::endian
{
  return mEndianness;
}

auto TileFormat::zlib_compression_level() const -> int
{
  return mZlibCompressionLevel;
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
  return level == -1 || (level >= 1 && level <= 9);
}

}  // namespace tactile