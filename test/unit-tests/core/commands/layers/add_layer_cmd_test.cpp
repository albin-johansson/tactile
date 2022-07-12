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

  ASSERT_EQ(1, contexts.context_count());
  ASSERT_TRUE(contexts.has_context(map.get_uuid()));

  AddLayerCmd cmd {document.get(), LayerType::TileLayer};

  cmd.redo();
  ASSERT_EQ(1, map.layer_count());
  ASSERT_EQ(2, contexts.context_count());
  ASSERT_TRUE(map.active_layer_id().has_value());

  const auto layerId = map.active_layer_id().value();
  ASSERT_TRUE(contexts.has_context(layerId));

  cmd.undo();
  ASSERT_EQ(0, map.layer_count());
  ASSERT_EQ(1, contexts.context_count());
  ASSERT_FALSE(map.active_layer_id().has_value());
  ASSERT_FALSE(contexts.has_context(layerId));
}

}  // namespace tactile::test
