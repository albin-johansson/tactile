// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/map/fix_tiles_in_map_cmd.hpp"

#include <gtest/gtest.h>

#include "core/layer/tile_layer.hpp"
#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(FixTilesInMapCmd, Constructor)
{
  ASSERT_THROW(FixTilesInMapCmd {nullptr}, TactileError);
}

TEST(FixTilesInMapCmd, RedoUndo)
{
  UUID layerId;
  UUID tilesetId;

  auto document = test::MapBuilder::build()  //
                      .with_size(10, 10)
                      .with_tile_layer(&layerId)
                      .with_tileset(&tilesetId)
                      .result();
  auto map = document->get_map_ptr();

  const auto& tilesetRef = map->get_tilesets().get_ref(tilesetId);

  auto& layer = map->view_tile_layer(layerId);
  layer.set_tile({2, 4}, tilesetRef.first_tile() - 10);
  layer.set_tile({0, 0}, tilesetRef.last_tile() + 1);
  layer.set_tile({0, 1}, tilesetRef.last_tile());
  layer.set_tile({5, 7}, tilesetRef.first_tile());

  FixTilesInMapCmd cmd {map};
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

}  // namespace tactile::test
