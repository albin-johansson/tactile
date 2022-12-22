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

#include "model/cmd/layer/add_layer.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "core/layer/group_layer.hpp"
#include "debug/panic.hpp"

namespace tactile::test {

TEST(AddLayer, Constructor)
{
  ASSERT_THROW(cmd::AddLayer(nullptr, LayerType::TileLayer), TactileError);
}

TEST(AddLayer, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto& map = document->get_map();
  auto& root = map.invisible_root();
  auto& contexts = document->get_contexts();

  ASSERT_EQ(1u, contexts.size());
  ASSERT_TRUE(contexts.contains(map.get_uuid()));

  cmd::AddLayer cmd {document.get(), LayerType::TileLayer};

  cmd.redo();
  ASSERT_EQ(1u, root.size());
  ASSERT_EQ(2u, contexts.size());
  ASSERT_TRUE(map.active_layer_id().has_value());

  const auto layer_id = map.active_layer_id().value();
  ASSERT_TRUE(contexts.contains(layer_id));

  cmd.undo();
  ASSERT_EQ(0u, root.size());
  ASSERT_EQ(1u, contexts.size());
  ASSERT_FALSE(map.active_layer_id().has_value());
  ASSERT_FALSE(contexts.contains(layer_id));
}

}  // namespace tactile::test
