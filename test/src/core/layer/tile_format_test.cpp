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

#include <doctest/doctest.h>
#include <zlib.h>
#include <zstd.h>

#include "tactile/core/debug/exception.hpp"

namespace tactile::test {

TEST_SUITE("TileFormat")
{
  TEST_CASE("Defaults")
  {
    const TileFormat format;

    REQUIRE(TileEncoding::Plain == format.encoding());
    REQUIRE(TileCompression::None == format.compression());

    REQUIRE(-1 == Z_DEFAULT_COMPRESSION);
    REQUIRE(-1 == format.zlib_compression_level());

    REQUIRE(3 == ZSTD_defaultCLevel());
    REQUIRE(ZSTD_defaultCLevel() == format.zstd_compression_level());
  }

  TEST_CASE("set_zlib_compression_level")
  {
    TileFormat format;

    format.set_zlib_compression_level(7);
    REQUIRE(7 == format.zlib_compression_level());

    REQUIRE_THROWS_AS(format.set_zlib_compression_level(-2), Exception);
    REQUIRE_THROWS_AS(format.set_zlib_compression_level(0), Exception);
    REQUIRE_THROWS_AS(format.set_zlib_compression_level(10), Exception);
  }

  TEST_CASE("can_use_compression_strategy (with plain encoding)")
  {
    TileFormat format;
    format.set_encoding(TileEncoding::Plain);

    REQUIRE(format.can_use_compression_strategy(TileCompression::None));
    REQUIRE(!format.can_use_compression_strategy(TileCompression::Zlib));
    REQUIRE(!format.can_use_compression_strategy(TileCompression::Zstd));
  }

  TEST_CASE("can_use_compression_strategy (ith Base64 encoding)")
  {
    TileFormat format;
    format.set_encoding(TileEncoding::Base64);

    REQUIRE(format.can_use_compression_strategy(TileCompression::None));
    REQUIRE(format.can_use_compression_strategy(TileCompression::Zlib));
    REQUIRE(format.can_use_compression_strategy(TileCompression::Zstd));
  }

  TEST_CASE("is_valid_zlib_compression_level")
  {
    // 0 is actually a valid level as far as Zlib is concerned (it indicates no
    // compression) But it doesn't make much sense for us to support Zlib compression at
    // level 0
    REQUIRE(!TileFormat::is_valid_zlib_compression_level(0));

    REQUIRE(!TileFormat::is_valid_zlib_compression_level(-2));
    REQUIRE(!TileFormat::is_valid_zlib_compression_level(10));

    REQUIRE(TileFormat::is_valid_zlib_compression_level(-1));
    REQUIRE(TileFormat::is_valid_zlib_compression_level(1));
    REQUIRE(TileFormat::is_valid_zlib_compression_level(9));

    REQUIRE(TileFormat::is_valid_zlib_compression_level(Z_DEFAULT_COMPRESSION));
    REQUIRE(TileFormat::is_valid_zlib_compression_level(Z_BEST_SPEED));
    REQUIRE(TileFormat::is_valid_zlib_compression_level(Z_BEST_COMPRESSION));

    for (int level = 1; level <= 9; ++level) {
      REQUIRE(TileFormat::is_valid_zlib_compression_level(level));
    }
  }

  TEST_CASE("is_valid_zstd_compression_level")
  {
    REQUIRE(!TileFormat::is_valid_zstd_compression_level(0));
    REQUIRE(!TileFormat::is_valid_zstd_compression_level(20));

    REQUIRE(TileFormat::is_valid_zstd_compression_level(ZSTD_defaultCLevel()));
    REQUIRE(TileFormat::is_valid_zstd_compression_level(1));
    REQUIRE(TileFormat::is_valid_zstd_compression_level(19));

    for (int level = 1; level <= 19; ++level) {
      REQUIRE(TileFormat::is_valid_zstd_compression_level(level));
    }
  }
}

}  // namespace tactile::test
