// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/layer/remove_layer_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(RemoveLayerCmd, Constructor)
{
  ASSERT_THROW(RemoveLayerCmd(nullptr, make_uuid()), TactileError);
}

TEST(RemoveLayerCmd, RedoUndo)
{
  UUID layerId;

  auto document = MapBuilder::build()  //
                      .with_object_layer(&layerId)
                      .result();

  auto& map = document->get_map();
  auto& contexts = document->get_contexts();

  ASSERT_TRUE(contexts.contains(layerId));

  RemoveLayerCmd cmd {document.get(), layerId};

  cmd.redo();
  ASSERT_EQ(0, map.layer_count());
  ASSERT_FALSE(contexts.contains(layerId));

  cmd.undo();
  ASSERT_EQ(1, map.layer_count());
  ASSERT_TRUE(contexts.contains(layerId));
}

}  // namespace tactile::test
