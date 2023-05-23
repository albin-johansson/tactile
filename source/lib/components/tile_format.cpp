/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

namespace tactile {

auto TileFormat::supports_compression() const -> bool
{
  return encoding != TileEncoding::Plain;
}

auto is_valid_zlib_compression_level(const int level) -> bool
{
  return level == Z_DEFAULT_COMPRESSION ||
         (level >= min_zlib_compression_level() && level <= max_zlib_compression_level());
}

auto is_valid_zstd_compression_level(const int level) -> bool
{
  return level >= min_zstd_compression_level() && level <= max_zstd_compression_level();
}

auto min_zlib_compression_level() -> int
{
  return Z_BEST_SPEED;
}

auto max_zlib_compression_level() -> int
{
  return Z_BEST_COMPRESSION;
}

auto min_zstd_compression_level() -> int
{
  return 1;
}

auto max_zstd_compression_level() -> int
{
  return 19;
}

}  // namespace tactile