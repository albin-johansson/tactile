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

#include "model/cmd/map/set_zstd_compression_level.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::SetZstdCompressionLevel")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::SetZstdCompressionLevel(nullptr, 0), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto map = map_document->get_map_ptr();

    cmd::SetZstdCompressionLevel cmd {map, 16};

    cmd.redo();
    REQUIRE(map->tile_format().zstd_compression_level() == 16);

    cmd.undo();
    REQUIRE(map->tile_format().zstd_compression_level() == 3);
  }

  TEST_CASE("merge_with")
  {
    auto map_document = MapBuilder::build().result();
    auto map = map_document->get_map_ptr();

    auto& format = map->tile_format();
    format.set_zstd_compression_level(8);

    cmd::SetZstdCompressionLevel a {map, 5};
    const cmd::SetZstdCompressionLevel b {map, 12};
    const cmd::SetZstdCompressionLevel c {map, 18};

    REQUIRE(a.merge_with(&b));
    REQUIRE(a.merge_with(&c));

    a.redo();
    REQUIRE(format.zstd_compression_level() == 18);

    a.undo();
    REQUIRE(format.zstd_compression_level() == 8);
  }
}

}  // namespace tactile::test
