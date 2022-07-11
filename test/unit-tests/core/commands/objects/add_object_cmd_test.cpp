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

#include "core/commands/objects/add_object_cmd.hpp"

#include <gtest/gtest.h>

#include "core/layers/object_layer.hpp"
#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(AddObjectCmd, Constructor)
{
  ASSERT_THROW(AddObjectCmd(nullptr, make_uuid(), ObjectType::Rect, {}), TactileError);
}

TEST(AddObjectCmd, RedoUndo)
{
  UUID  layerId;
  auto  document = test::MapBuilder::build().with_object_layer(&layerId).result();
  auto& layer = document->get_map().view_object_layer(layerId);

  AddObjectCmd cmd {document.get(), layerId, ObjectType::Point, {0, 0}};

  cmd.redo();
  ASSERT_EQ(1, layer.object_count());

  auto       object = layer.begin()->second;
  const auto objectId = object->get_uuid();
  ASSERT_TRUE(document->has_context(objectId));

  cmd.undo();
  ASSERT_EQ(0, layer.object_count());
  ASSERT_FALSE(document->has_context(objectId));
}

}  // namespace tactile::test
