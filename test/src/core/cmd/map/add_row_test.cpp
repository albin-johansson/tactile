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

#include "core/cmd/map/add_row.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "misc/panic.hpp"

namespace tactile::test {

TEST(AddRow, Constructor)
{
  ASSERT_THROW(cmd::AddRow {nullptr}, TactileError);
}

TEST(AddRow, RedoUndo)
{
  const usize initial_rows = 10;
  const usize initial_cols = 7;

  auto document =
      test::MapBuilder::build().with_size(initial_rows, initial_cols).result();
  auto map = document->get_map_ptr();

  cmd::AddRow cmd {map};
  cmd.redo();

  ASSERT_EQ(initial_rows + 1, map->row_count());
  ASSERT_EQ(initial_cols, map->column_count());

  cmd.undo();

  ASSERT_EQ(initial_rows, map->row_count());
  ASSERT_EQ(initial_cols, map->column_count());
}

TEST(AddRow, MergeSupport)
{
  const usize initial_rows = 6;
  const usize initial_cols = 7;

  auto document =
      test::MapBuilder::build().with_size(initial_rows, initial_cols).result();
  auto map = document->get_map_ptr();

  cmd::AddRow a {map};
  const cmd::AddRow b {map};

  ASSERT_TRUE(a.merge_with(&b));

  a.redo();

  ASSERT_EQ(initial_rows + 2, map->row_count());
  ASSERT_EQ(initial_cols, map->column_count());

  a.undo();

  ASSERT_EQ(initial_rows, map->row_count());
  ASSERT_EQ(initial_cols, map->column_count());
}

}  // namespace tactile::test
