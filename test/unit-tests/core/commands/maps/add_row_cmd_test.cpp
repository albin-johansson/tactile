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

#include "core/commands/maps/add_row_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(AddRowCmd, Constructor)
{
  ASSERT_THROW(AddRowCmd {nullptr}, TactileError);
}

TEST(AddRowCmd, RedoUndo)
{
  const usize initialRows = 10;
  const usize initialCols = 7;

  auto document = test::MapBuilder::build().with_size(initialRows, initialCols).result();
  auto map = document->get_map_ptr();

  AddRowCmd cmd {map};
  cmd.redo();

  ASSERT_EQ(initialRows + 1, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());

  cmd.undo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());
}

TEST(AddRowCmd, MergeSupport)
{
  const usize initialRows = 6;
  const usize initialCols = 7;

  auto document = test::MapBuilder::build().with_size(initialRows, initialCols).result();
  auto map = document->get_map_ptr();

  AddRowCmd       a {map};
  const AddRowCmd b {map};

  ASSERT_TRUE(a.merge_with(&b));

  a.redo();

  ASSERT_EQ(initialRows + 2, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());

  a.undo();

  ASSERT_EQ(initialRows, map->row_count());
  ASSERT_EQ(initialCols, map->column_count());
}

}  // namespace tactile::test
