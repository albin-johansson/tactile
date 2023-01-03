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
    const usize initial_rows = 5;
    const usize initial_cols = 7;

    auto map_document = test::MapBuilder::build()  //
                            .with_size(initial_rows, initial_cols)
                            .result();
    auto map = map_document->get_map_ptr();

    cmd::AddColumn cmd {map};

    cmd.redo();
    REQUIRE(initial_rows == map->row_count());
    REQUIRE(initial_cols + 1 == map->column_count());

    cmd.undo();
    REQUIRE(initial_rows == map->row_count());
    REQUIRE(initial_cols == map->column_count());
  }

  TEST_CASE("merge_with")
  {
    const usize initial_rows = 13;
    const usize initial_cols = 5;

    auto map_document = test::MapBuilder::build()  //
                            .with_size(initial_rows, initial_cols)
                            .result();
    auto map = map_document->get_map_ptr();

    cmd::AddColumn a {map};
    const cmd::AddColumn b {map};
    const cmd::AddColumn c {map};

    REQUIRE(a.merge_with(&b));
    REQUIRE(a.merge_with(&c));

    a.redo();
    REQUIRE(initial_rows == map->row_count());
    REQUIRE(initial_cols + 3 == map->column_count());

    a.undo();
    REQUIRE(initial_rows == map->row_count());
    REQUIRE(initial_cols == map->column_count());
  }
}

}  // namespace tactile::test
