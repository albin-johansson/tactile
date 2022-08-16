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

#include "core/layer/tile_format.hpp"

#include <gtest/gtest.h>
#include <zlib.h>
#include <zstd.h>

#include "misc/panic.hpp"

namespace tactile::test {

TEST(TileFormat, Defaults)
{
  const TileFormat format;

  ASSERT_EQ(TileEncoding::Plain, format.encoding());
  ASSERT_EQ(TileCompression::None, format.compression());
  ASSERT_EQ(std::endian::native, format.endianness());

  ASSERT_EQ(-1, Z_DEFAULT_COMPRESSION);
  ASSERT_EQ(-1, format.zlib_compression_level());

  ASSERT_EQ(3, ZSTD_defaultCLevel());
  ASSERT_EQ(ZSTD_defaultCLevel(), format.zstd_compression_level());
}

TEST(TileFormat, SetEndianness)
{
  TileFormat format;

  format.set_endianness(std::endian::little);
  ASSERT_EQ(std::endian::little, format.endianness());

  format.set_endianness(std::endian::big);
  ASSERT_EQ(std::endian::big, format.endianness());
}

TEST(TileFormat, SetZlibCompressionLevel)
{
  TileFormat format;

  format.set_zlib_compression_level(7);
  ASSERT_EQ(7, format.zlib_compression_level());

  ASSERT_THROW(format.set_zlib_compression_level(-2), TactileError);
  ASSERT_THROW(format.set_zlib_compression_level(0), TactileError);
  ASSERT_THROW(format.set_zlib_compression_level(10), TactileError);
}

TEST(TileFormat, CanUseCompressionStrategyWithPlainEncoding)
{
  TileFormat format;
  format.set_encoding(TileEncoding::Plain);

  ASSERT_TRUE(format.can_use_compression_strategy(TileCompression::None));
  ASSERT_FALSE(format.can_use_compression_strategy(TileCompression::Zlib));
  ASSERT_FALSE(format.can_use_compression_strategy(TileCompression::Zstd));
}

TEST(TileFormat, CanUseCompressionStrategyWithBase64Encoding)
{
  TileFormat format;
  format.set_encoding(TileEncoding::Base64);

  ASSERT_TRUE(format.can_use_compression_strategy(TileCompression::None));
  ASSERT_TRUE(format.can_use_compression_strategy(TileCompression::Zlib));
  ASSERT_TRUE(format.can_use_compression_strategy(TileCompression::Zstd));
}

TEST(TileFormat, IsValidZlibCompressionLevel)
{
  // 0 is actually a valid level as far as Zlib is concerned (it indicates no compression)
  // But it doesn't make much sense for us to support Zlib compression at level 0
  ASSERT_FALSE(TileFormat::is_valid_zlib_compression_level(0));

  ASSERT_FALSE(TileFormat::is_valid_zlib_compression_level(-2));
  ASSERT_FALSE(TileFormat::is_valid_zlib_compression_level(10));

  ASSERT_TRUE(TileFormat::is_valid_zlib_compression_level(-1));
  ASSERT_TRUE(TileFormat::is_valid_zlib_compression_level(1));
  ASSERT_TRUE(TileFormat::is_valid_zlib_compression_level(9));

  ASSERT_TRUE(TileFormat::is_valid_zlib_compression_level(Z_DEFAULT_COMPRESSION));
  ASSERT_TRUE(TileFormat::is_valid_zlib_compression_level(Z_BEST_SPEED));
  ASSERT_TRUE(TileFormat::is_valid_zlib_compression_level(Z_BEST_COMPRESSION));

  for (int level = 1; level <= 9; ++level) {
    ASSERT_TRUE(TileFormat::is_valid_zlib_compression_level(level));
  }
}

TEST(TileFormat, IsValidZstdCompressionLevel)
{
  ASSERT_FALSE(TileFormat::is_valid_zstd_compression_level(ZSTD_minCLevel() - 1));
  ASSERT_FALSE(TileFormat::is_valid_zstd_compression_level(ZSTD_maxCLevel() + 1));

  ASSERT_TRUE(TileFormat::is_valid_zstd_compression_level(ZSTD_defaultCLevel()));
  ASSERT_TRUE(TileFormat::is_valid_zstd_compression_level(ZSTD_minCLevel()));
  ASSERT_TRUE(TileFormat::is_valid_zstd_compression_level(ZSTD_maxCLevel()));
}

}  // namespace tactile::test