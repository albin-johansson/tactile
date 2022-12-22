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

#include "model/cmd/layer/set_layer_opacity.hpp"

#include <gtest/gtest.h>

#include "core/layer/tile_layer.hpp"
#include "debug/panic.hpp"

namespace tactile::test {

TEST(SetLayerOpacity, Constructor)
{
  ASSERT_THROW(cmd::SetLayerOpacity(nullptr, 1.0f), TactileError);
}

TEST(SetLayerOpacity, RedoUndo)
{
  auto layer = std::make_shared<TileLayer>();
  ASSERT_EQ(1.0f, layer->opacity());

  cmd::SetLayerOpacity cmd {layer, 0.8f};

  cmd.redo();
  ASSERT_EQ(0.8f, layer->opacity());

  cmd.undo();
  ASSERT_EQ(1.0f, layer->opacity());
}

TEST(SetLayerOpacity, MergeSupport)
{
  auto layer = std::make_shared<TileLayer>();

  cmd::SetLayerOpacity a {layer, 0.8f};
  const cmd::SetLayerOpacity b {layer, 0.6f};
  const cmd::SetLayerOpacity c {layer, 0.4f};

  ASSERT_TRUE(a.merge_with(&b));
  ASSERT_TRUE(a.merge_with(&c));

  a.redo();
  ASSERT_EQ(0.4f, layer->opacity());

  a.undo();
  ASSERT_EQ(1.0f, layer->opacity());
}

}  // namespace tactile::test
