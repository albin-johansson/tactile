// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/layer/set_layer_opacity_cmd.hpp"

#include <gtest/gtest.h>

#include "core/layer/tile_layer.hpp"
#include "misc/panic.hpp"

namespace tactile::test {

TEST(SetLayerOpacityCmd, Constructor)
{
  ASSERT_THROW(SetLayerOpacityCmd(nullptr, 1.0f), TactileError);
}

TEST(SetLayerOpacityCmd, RedoUndo)
{
  auto layer = std::make_shared<TileLayer>();
  ASSERT_EQ(1.0f, layer->get_opacity());

  SetLayerOpacityCmd cmd {layer, 0.8f};

  cmd.redo();
  ASSERT_EQ(0.8f, layer->get_opacity());

  cmd.undo();
  ASSERT_EQ(1.0f, layer->get_opacity());
}

TEST(SetLayerOpacityCmd, MergeSupport)
{
  auto layer = std::make_shared<TileLayer>();

  SetLayerOpacityCmd       a {layer, 0.8f};
  const SetLayerOpacityCmd b {layer, 0.6f};
  const SetLayerOpacityCmd c {layer, 0.4f};

  ASSERT_TRUE(a.merge_with(&b));
  ASSERT_TRUE(a.merge_with(&c));

  a.redo();
  ASSERT_EQ(0.4f, layer->get_opacity());

  a.undo();
  ASSERT_EQ(1.0f, layer->get_opacity());
}

}  // namespace tactile::test
