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

#include "model/cmd/map/set_tile_format_compression.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::SetTileFormatCompression")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(SetTileFormatCompression(nullptr, TileCompression::None),
                      TactileError);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto map = map_document->get_map_ptr();

    const auto encoding = TileEncoding::Base64;
    const auto old_compression = TileCompression::None;
    const auto new_compression = TileCompression::Zlib;

    auto& format = map->get_tile_format();
    format.set_encoding(encoding);
    format.set_compression(old_compression);

    REQUIRE(format.encoding() == encoding);
    REQUIRE(format.compression() == old_compression);

    SetTileFormatCompression cmd {map, new_compression};

    cmd.redo();
    REQUIRE(format.compression() == new_compression);
    REQUIRE(format.encoding() == encoding);

    cmd.undo();
    REQUIRE(format.compression() == old_compression);
    REQUIRE(format.encoding() == encoding);
  }
}

}  // namespace tactile::test
