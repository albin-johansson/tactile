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

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "debug/panic.hpp"

namespace tactile::test {

TEST(ResizeMap, Constructor)
{
  ASSERT_THROW(cmd::ResizeMap(nullptr, 1, 1), TactileError);
}

TEST(ResizeMap, RedoUndo)
{
  auto document = test::MapBuilder::build().with_size(5, 7).result();
  auto map = document->get_map_ptr();

  cmd::ResizeMap cmd {map, 3, 9};
  cmd.redo();

  ASSERT_EQ(3u, map->row_count());
  ASSERT_EQ(9u, map->column_count());

  cmd.undo();

  ASSERT_EQ(5u, map->row_count());
  ASSERT_EQ(7u, map->column_count());
}

}  // namespace tactile::test
