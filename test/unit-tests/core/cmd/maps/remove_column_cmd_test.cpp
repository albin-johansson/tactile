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

#include "core/cmd/map/remove_column_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"
#include "unit-tests/core/helpers/map_test_helpers.hpp"

namespace tactile::test {

TEST(RemoveColumnCmd, Constructor)
{
  ASSERT_THROW(RemoveColumnCmd {nullptr}, TactileError);
}

TEST(RemoveColumnCmd, RedoUndo)
{
  const usize initialRows = 4;
  const usize initialCols = 6;

  UUID layerId;

  auto document = test::MapBuilder::build()  //
                      .with_size(initialRows, initialCols)
                      .with_tile_layer(&layerId, 42)
                      .result();
  auto map = document->get_map_ptr();

  RemoveColumnCmd cmd {map};
  cmd.redo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols - 1, map->column_count());

  cmd.undo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());

  const auto& layer = map->view_tile_layer(layerId);
  test::verify_all_tiles_matches(layer, 42);
}

TEST(RemoveColumnCmd, MergeSupport)
{
  const usize initialRows = 7;
  const usize initialCols = 5;

  auto document = test::MapBuilder::build()  //
                      .with_size(initialRows, initialCols)
                      .with_tile_layer()
                      .result();
  auto map = document->get_map_ptr();

  RemoveColumnCmd       a {map};
  const RemoveColumnCmd b {map};

  ASSERT_TRUE(a.merge_with(&b));

  a.redo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols - 2, map->column_count());

  a.undo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());
}

}  // namespace tactile::test
