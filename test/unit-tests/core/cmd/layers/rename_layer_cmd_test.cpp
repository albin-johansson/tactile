// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/layer/rename_layer_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(RenameLayerCmd, Constructor)
{
  ASSERT_THROW(RenameLayerCmd(nullptr, make_uuid(), ""), TactileError);
}

TEST(RenameLayerCmd, RedoUndo)
{
  UUID layerId;

  auto document = MapBuilder::build().with_tile_layer(&layerId).result();
  auto map = document->get_map_ptr();

  auto layer = map->get_layer(layerId);
  layer->set_name("barfoo");

  RenameLayerCmd cmd {map, layerId, "foobar"};

  cmd.redo();
  ASSERT_EQ("foobar", layer->get_name());

  cmd.undo();
  ASSERT_EQ("barfoo", layer->get_name());
}

}  // namespace tactile::test
