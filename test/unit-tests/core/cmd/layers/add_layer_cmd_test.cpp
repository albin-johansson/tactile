// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/layer/add_layer_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(AddLayerCmd, Constructor)
{
  ASSERT_THROW(AddLayerCmd(nullptr, LayerType::TileLayer), TactileError);
}

TEST(AddLayerCmd, RedoUndo)
{
  auto  document = MapBuilder::build().result();
  auto& map = document->get_map();
  auto& contexts = document->get_contexts();

  ASSERT_EQ(1, contexts.size());
  ASSERT_TRUE(contexts.contains(map.get_uuid()));

  AddLayerCmd cmd {document.get(), LayerType::TileLayer};

  cmd.redo();
  ASSERT_EQ(1, map.layer_count());
  ASSERT_EQ(2, contexts.size());
  ASSERT_TRUE(map.active_layer_id().has_value());

  const auto layerId = map.active_layer_id().value();
  ASSERT_TRUE(contexts.contains(layerId));

  cmd.undo();
  ASSERT_EQ(0, map.layer_count());
  ASSERT_EQ(1, contexts.size());
  ASSERT_FALSE(map.active_layer_id().has_value());
  ASSERT_FALSE(contexts.contains(layerId));
}

}  // namespace tactile::test
