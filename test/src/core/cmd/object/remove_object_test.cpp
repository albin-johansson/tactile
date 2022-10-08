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

#include "core/cmd/object/remove_object.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "core/layer/object_layer.hpp"
#include "misc/panic.hpp"

namespace tactile::test {

TEST(RemoveObject, Constructor)
{
  ASSERT_THROW(cmd::RemoveObject(nullptr, make_uuid(), make_uuid()), TactileError);
}

TEST(RemoveObject, RedoUndo)
{
  UUID layer_id;
  Shared<Object> object;

  auto document = MapBuilder::build()  //
                      .with_object(ObjectType::Rect, &object, &layer_id)
                      .result();

  auto map = document->get_map_ptr();
  auto& contexts = document->get_contexts();

  auto& layer = map->invisible_root().view_object_layer(layer_id);

  cmd::RemoveObject cmd {document.get(), layer_id, object->get_uuid()};

  cmd.redo();
  ASSERT_FALSE(contexts.contains(object->get_uuid()));
  ASSERT_FALSE(layer.has_object(object->get_uuid()));

  cmd.undo();
  ASSERT_TRUE(contexts.contains(object->get_uuid()));
  ASSERT_TRUE(layer.has_object(object->get_uuid()));
}

}  // namespace tactile::test