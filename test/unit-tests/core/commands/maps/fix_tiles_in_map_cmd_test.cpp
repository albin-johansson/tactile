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

#include "core/commands/maps/fix_tiles_in_map_cmd.hpp"

#include <gtest/gtest.h>

#include "core/layers/tile_layer.hpp"
#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

using namespace tactile;

TEST(FixTilesInMapCmd, Constructor)
{
  ASSERT_THROW(FixTilesInMapCmd{nullptr}, TactileError);
}

TEST(FixTilesInMapCmd, RedoUndo)
{
  UUID layerId;
  UUID tilesetId;

  auto map = test::MapBuilder::build()  //
                 .with_size(10, 10)
                 .with_tile_layer(&layerId)
                 .with_tileset(&tilesetId)
                 .result();

  const auto& tilesetRef = map->get_tilesets().get_ref(tilesetId);

  auto& layer = map->view_tile_layer(layerId);
  layer.set_tile({2, 4}, tilesetRef.first_tile() - 10);
  layer.set_tile({0, 0}, tilesetRef.last_tile() + 1);
  layer.set_tile({0, 1}, tilesetRef.last_tile());
  layer.set_tile({5, 7}, tilesetRef.first_tile());

  FixTilesInMapCmd cmd{map};
  cmd.redo();

  ASSERT_EQ(empty_tile, layer.tile_at({2, 4}));
  ASSERT_EQ(empty_tile, layer.tile_at({0, 0}));
  ASSERT_EQ(tilesetRef.last_tile(), layer.tile_at({0, 1}));
  ASSERT_EQ(tilesetRef.first_tile(), layer.tile_at({5, 7}));

  cmd.undo();

  ASSERT_EQ(tilesetRef.first_tile() - 10, layer.tile_at({2, 4}));
  ASSERT_EQ(tilesetRef.last_tile() + 1, layer.tile_at({0, 0}));
  ASSERT_EQ(tilesetRef.last_tile(), layer.tile_at({0, 1}));
  ASSERT_EQ(tilesetRef.first_tile(), layer.tile_at({5, 7}));
}