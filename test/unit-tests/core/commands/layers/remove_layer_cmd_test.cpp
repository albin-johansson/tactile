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
