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

#include "model/cmd/map/fix_map_tiles.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "core/debug/panic.hpp"

namespace tactile::test {

TEST(FixMapTiles, Constructor)
{
  ASSERT_THROW(cmd::FixMapTiles {nullptr}, TactileError);
}

TEST(FixMapTiles, RedoUndo)
{
  UUID layer_id;
  UUID tileset_id;

  auto document = test::MapBuilder::build()  //
                      .with_size(10, 10)
                      .with_tile_layer(&layer_id)
                      .with_tileset(&tileset_id)
                      .result();
  auto map = document->get_map_ptr();

  const auto& tileset_ref = map->tileset_bundle().get_ref(tileset_id);

  auto& layer = map->invisible_root().get_tile_layer(layer_id);
  layer.set_tile({2, 4}, tileset_ref.get_first_tile() - 10);
  layer.set_tile({0, 0}, tileset_ref.get_last_tile() + 1);
  layer.set_tile({0, 1}, tileset_ref.get_last_tile());
  layer.set_tile({5, 7}, tileset_ref.get_first_tile());

  cmd::FixMapTiles cmd {map};
  cmd.redo();

  ASSERT_EQ(empty_tile, layer.tile_at({2, 4}));
  ASSERT_EQ(empty_tile, layer.tile_at({0, 0}));
  ASSERT_EQ(tileset_ref.get_last_tile(), layer.tile_at({0, 1}));
  ASSERT_EQ(tileset_ref.get_first_tile(), layer.tile_at({5, 7}));

  cmd.undo();

  ASSERT_EQ(tileset_ref.get_first_tile() - 10, layer.tile_at({2, 4}));
  ASSERT_EQ(tileset_ref.get_last_tile() + 1, layer.tile_at({0, 0}));
  ASSERT_EQ(tileset_ref.get_last_tile(), layer.tile_at({0, 1}));
  ASSERT_EQ(tileset_ref.get_first_tile(), layer.tile_at({5, 7}));
}

}  // namespace tactile::test
