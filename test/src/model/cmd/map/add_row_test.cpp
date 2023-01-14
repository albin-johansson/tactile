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

#include "model/cmd/map/add_row.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::AddRow")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::AddRow {nullptr}, TactileError);
  }

  TEST_CASE("redo/undo")
  {
    const usize initial_rows = 10;
    const usize initial_cols = 7;

    auto map_document = test::MapBuilder::build()  //
                            .with_size(initial_rows, initial_cols)
                            .result();
    auto map = map_document->get_map_ptr();

    cmd::AddRow cmd {map};

    cmd.redo();
    REQUIRE(initial_rows + 1 == map->get_extent().rows);
    REQUIRE(initial_cols == map->get_extent().cols);

    cmd.undo();
    REQUIRE(initial_rows == map->get_extent().rows);
    REQUIRE(initial_cols == map->get_extent().cols);
  }

  TEST_CASE("merge_with")
  {
    const usize initial_rows = 6;
    const usize initial_cols = 7;

    auto map_document = test::MapBuilder::build()  //
                            .with_size(initial_rows, initial_cols)
                            .result();
    auto map = map_document->get_map_ptr();

    cmd::AddRow a {map};
    const cmd::AddRow b {map};

    REQUIRE(a.merge_with(&b));

    a.redo();
    REQUIRE(initial_rows + 2 == map->get_extent().rows);
    REQUIRE(initial_cols == map->get_extent().cols);

    a.undo();
    REQUIRE(initial_rows == map->get_extent().rows);
    REQUIRE(initial_cols == map->get_extent().cols);
  }
}

}  // namespace tactile::test
