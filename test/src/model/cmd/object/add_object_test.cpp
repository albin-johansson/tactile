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

#include "model/cmd/object/add_object.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "debug/panic.hpp"

namespace tactile::test {

TEST(AddObject, Constructor)
{
  ASSERT_THROW(cmd::AddObject(nullptr, make_uuid(), ObjectType::Rect, {}), TactileError);
}

TEST(AddObject, RedoUndo)
{
  UUID layer_id;
  auto document = test::MapBuilder::build().with_object_layer(&layer_id).result();
  auto& layer = document->get_map().invisible_root().object_layer(layer_id);

  cmd::AddObject cmd {document.get(), layer_id, ObjectType::Point, {0, 0}};

  cmd.redo();
  ASSERT_EQ(1u, layer.object_count());

  auto object = layer.begin()->second;
  const auto object_id = object->get_uuid();
  ASSERT_TRUE(document->get_contexts().contains(object_id));

  cmd.undo();
  ASSERT_EQ(0u, layer.object_count());
  ASSERT_FALSE(document->get_contexts().contains(object_id));
}

}  // namespace tactile::test
