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

#include "model/cmd/map/set_tile_format_encoding.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::SetTileFormatEncoding")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(SetTileFormatEncoding(nullptr, TileEncoding::Plain), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto map = map_document->get_map_ptr();

    auto& format = map->tile_format();

    const auto old_encoding = TileEncoding::Plain;
    const auto new_encoding = TileEncoding::Base64;
    REQUIRE(format.encoding() == old_encoding);

    SetTileFormatEncoding cmd {map, new_encoding};

    cmd.redo();
    REQUIRE(format.encoding() == new_encoding);

    cmd.undo();
    REQUIRE(format.encoding() == old_encoding);
  }
}

}  // namespace tactile::test
