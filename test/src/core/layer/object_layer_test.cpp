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

#include "core/layer/object_layer.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

TEST(ObjectLayer, Defaults)
{
  const ObjectLayer layer;
  ASSERT_EQ(LayerType::ObjectLayer, layer.type());
  ASSERT_EQ(0, layer.object_count());
  ASSERT_FALSE(layer.meta_id().has_value());
}

TEST(ObjectLayer, AddObject)
{
  ObjectLayer layer;
  auto object = std::make_shared<Object>();

  ASSERT_EQ(0, layer.object_count());
  ASSERT_FALSE(layer.has_object(object->get_uuid()));

  layer.add_object(object);

  ASSERT_EQ(1, layer.object_count());
  ASSERT_TRUE(layer.has_object(object->get_uuid()));
}

TEST(ObjectLayer, RemoveObject)
{
  ObjectLayer layer;
  auto object = std::make_shared<Object>();

  ASSERT_NO_THROW(layer.remove_object(object->get_uuid()));
}

}  // namespace tactile::test
