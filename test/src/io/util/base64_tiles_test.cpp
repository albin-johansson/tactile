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

#include "io/util/base64_tiles.hpp"

#include <doctest/doctest.h>

namespace tactile::test {

TEST_SUITE("Base64 tiles")
{
  TEST_CASE("Encode/decode roundtrip")
  {
    const TileExtent extent {3, 3};
    const TileMatrix source {{1000, 2000, 3000}, {4000, 5000, 6000}, {7000, 8000, 9000}};

    const auto compression_mode = TileCompression::Zlib;
    const auto encoded = base64_encode_tiles(source, extent, compression_mode);
    const auto decoded = base64_decode_tiles(encoded, extent, compression_mode);

    REQUIRE(source == decoded);
  }
}

}  // namespace tactile::test
