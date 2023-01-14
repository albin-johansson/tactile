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

#include "model/cmd/map/add_column.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::AddColumn")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::AddColumn {nullptr}, TactileError);
  }

  TEST_CASE("redo/undo")
  {
    const TileExtent initial_extent {5, 7};

    auto map_document = test::MapBuilder::build()  //
                            .with_size(initial_extent.rows, initial_extent.cols)
                            .result();
    auto map = map_document->get_map_ptr();

    cmd::AddColumn cmd {map};

    cmd.redo();
    REQUIRE(initial_extent.rows == map->get_extent().rows);
    REQUIRE(initial_extent.cols + 1 == map->get_extent().cols);

    cmd.undo();
    REQUIRE(initial_extent == map->get_extent());
  }

  TEST_CASE("merge_with")
  {
    const TileExtent initial_extent {13, 5};

    auto map_document = test::MapBuilder::build()  //
                            .with_size(initial_extent.rows, initial_extent.cols)
                            .result();
    auto map = map_document->get_map_ptr();

    cmd::AddColumn a {map};
    const cmd::AddColumn b {map};
    const cmd::AddColumn c {map};

    REQUIRE(a.merge_with(&b));
    REQUIRE(a.merge_with(&c));

    a.redo();
    REQUIRE(initial_extent.rows == map->get_extent().rows);
    REQUIRE(initial_extent.cols + 3 == map->get_extent().cols);

    a.undo();
    REQUIRE(initial_extent.rows == map->get_extent().rows);
    REQUIRE(initial_extent.cols == map->get_extent().cols);
  }
}

}  // namespace tactile::test
