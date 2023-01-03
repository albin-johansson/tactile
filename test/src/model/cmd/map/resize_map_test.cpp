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

#include "model/cmd/map/resize_map.hpp"

#include <doctest/doctest.h>

#include "core/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::ResizeMap")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::ResizeMap(nullptr, 1, 1), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    const usize old_rows = 5;
    const usize old_cols = 7;

    auto map_document = test::MapBuilder::build()  //
                            .with_size(old_rows, old_cols)
                            .result();
    auto map = map_document->get_map_ptr();

    const usize new_rows = 3;
    const usize new_cols = 9;
    cmd::ResizeMap cmd {map, new_rows, new_cols};

    cmd.redo();
    REQUIRE(new_rows == map->row_count());
    REQUIRE(new_cols == map->column_count());

    cmd.undo();
    REQUIRE(old_rows == map->row_count());
    REQUIRE(old_cols == map->column_count());
  }
}

}  // namespace tactile::test
