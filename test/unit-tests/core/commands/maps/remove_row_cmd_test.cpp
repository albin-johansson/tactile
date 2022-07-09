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

#include "core/commands/maps/remove_row_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"
#include "unit-tests/core/helpers/map_test_helpers.hpp"

using namespace tactile;

TEST(RemoveRowCmd, Constructor)
{
  ASSERT_THROW(RemoveRowCmd{nullptr}, TactileError);
}

TEST(RemoveRowCmd, RedoUndo)
{
  const usize initialRows = 3;
  const usize initialCols = 5;

  auto document = test::MapBuilder::build().with_size(initialRows, initialCols).result();
  auto map = document->get_map_ptr();

  RemoveRowCmd cmd{map};
  cmd.redo();

  ASSERT_EQ(initialRows - 1, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());

  cmd.undo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());
}

TEST(RemoveRowCmd, MergeSupport)
{
  const usize initialRows = 3;
  const usize initialCols = 6;

  UUID layerId;

  auto document = test::MapBuilder::build()  //
                      .with_size(initialRows, initialCols)
                      .with_tile_layer(&layerId, 42)
                      .result();
  auto map = document->get_map_ptr();

  RemoveRowCmd       a{map};
  const RemoveRowCmd b{map};

  ASSERT_TRUE(a.merge_with(&b));

  a.redo();

  ASSERT_EQ(initialRows - 2, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());

  a.undo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());

  test::verify_all_tiles_matches(map->view_tile_layer(layerId), 42);
}