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

#include "model/cmd/map/set_zlib_compression_level.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::SetZlibCompressionLevel")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::SetZlibCompressionLevel(nullptr, 0), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto map = map_document->get_map_ptr();
    auto& format = map->tile_format();

    const int old_level = format.zlib_compression_level();
    const int new_level = 4;

    cmd::SetZlibCompressionLevel cmd {map, new_level};

    cmd.redo();
    REQUIRE(format.zlib_compression_level() == new_level);

    cmd.undo();
    REQUIRE(format.zlib_compression_level() == old_level);
  }

  TEST_CASE("merge_with")
  {
    auto map_document = MapBuilder::build().result();
    auto map = map_document->get_map_ptr();
    auto& format = map->tile_format();

    format.set_zlib_compression_level(7);

    cmd::SetZlibCompressionLevel a {map, 6};
    const cmd::SetZlibCompressionLevel b {map, 4};
    const cmd::SetZlibCompressionLevel c {map, 9};

    REQUIRE(a.merge_with(&b));
    REQUIRE(a.merge_with(&c));

    a.redo();
    REQUIRE(format.zlib_compression_level() == 9);

    a.undo();
    REQUIRE(format.zlib_compression_level() == 7);
  }
}

}  // namespace tactile::test
