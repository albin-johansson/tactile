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

#include "core/commands/maps/resize_map_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

using namespace tactile;

TEST(ResizeMapCmd, Constructor)
{
  ASSERT_THROW(ResizeMapCmd(nullptr, 1, 1), TactileError);
}

TEST(ResizeMapCmd, RedoUndo)
{
  auto map = test::MapBuilder::build()  //
                 .with_size(5, 7)
                 .result();

  ResizeMapCmd cmd{map, 3, 9};
  cmd.redo();

  ASSERT_EQ(3, map->row_count());
  ASSERT_EQ(9, map->column_count());

  cmd.undo();

  ASSERT_EQ(5, map->row_count());
  ASSERT_EQ(7, map->column_count());
}