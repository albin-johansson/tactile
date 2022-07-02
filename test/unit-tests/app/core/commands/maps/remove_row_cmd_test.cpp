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

#include "core/documents/map_document.hpp"
#include "core/layers/tile_layer.hpp"
#include "misc/panic.hpp"

using namespace tactile;

TEST(RemoveRowCmd, Constructor)
{
  ASSERT_THROW(RemoveRowCmd{nullptr}, TactileError);
}

TEST(RemoveRowCmd, RedoUndo)
{
  const usize initialRows = 3;
  const usize initialCols = 5;

  MapDocument document{{32, 32}, initialRows, initialCols};
  auto&       map = document.get_map();

  RemoveRowCmd cmd{&document};

  cmd.redo();

  ASSERT_EQ(initialRows - 1, map.row_count());
  ASSERT_EQ(initialCols, map.column_count());

  cmd.undo();

  ASSERT_EQ(initialRows, map.row_count());
  ASSERT_EQ(initialCols, map.column_count());
}

TEST(RemoveRowCmd, MergeSupport)
{
  const usize initialRows = 3;
  const usize initialCols = 6;

  MapDocument document{{32, 32}, initialRows, initialCols};
  auto&       map = document.get_map();

  const auto layerId = map.add_tile_layer();

  {
    auto& layer = map.view_tile_layer(layerId);
    layer.set_tile({0, 0}, 42);
    layer.set_tile({2, 4}, 23);
  }

  RemoveRowCmd a{&document};
  RemoveRowCmd b{&document};

  ASSERT_TRUE(a.merge_with(b));

  a.redo();

  ASSERT_EQ(initialRows - 2, map.row_count());
  ASSERT_EQ(initialCols, map.column_count());

  a.undo();

  ASSERT_EQ(initialRows, map.row_count());
  ASSERT_EQ(initialCols, map.column_count());

  {
    auto& layer = map.view_tile_layer(layerId);
    layer.set_tile({0, 0}, 42);
    layer.set_tile({2, 4}, 23);
    ASSERT_EQ(42, layer.tile_at({0, 0}));
    ASSERT_EQ(23, layer.tile_at({2, 4}));
  }
}