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

#include "core/systems/layers/object_system.hpp"

#include <gtest/gtest.h>

#include "core/common/ecs.hpp"
#include "core/components/attributes.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/systems/registry_system.hpp"
#include "misc/panic.hpp"

using namespace tactile;

namespace {

struct TestData final
{
  entt::registry registry;
  UUID layer_id{};
};

[[nodiscard]] auto _new_test_data() -> TestData
{
  TestData data{.registry = sys::new_map_document_registry()};

  const auto layerEntity = sys::new_object_layer(data.registry);
  const auto& context = checked_get<comp::Context>(data.registry, layerEntity);

  data.layer_id = context.id;

  return data;
}

/* Verifies the integrity of a newly created object */
void _verify_new_object(const TestData& data,
                        const ObjectID id,
                        const ObjectType type,
                        const Vector2f& pos,
                        const Vector2f& size)
{
  const auto objectEntity = sys::get_object(data.registry, id);

  ASSERT_TRUE(data.registry.all_of<comp::Object>(objectEntity));
  ASSERT_TRUE(data.registry.all_of<comp::Context>(objectEntity));

  const auto& object = checked_get<comp::Object>(data.registry, objectEntity);

  ASSERT_EQ(id, object.id);
  ASSERT_EQ(type, object.type);

  ASSERT_EQ(pos, object.pos);
  ASSERT_EQ(size, object.size);

  ASSERT_TRUE(object.visible);
  ASSERT_TRUE(object.tag.empty());
}

}  // namespace

TEST(ObjectSystem, NewRectangleObject)
{
  auto data = _new_test_data();

  const Vector2f pos{84.3f, 748.9f};
  const Vector2f size{432.6f, 254.0f};

  const auto id = sys::new_rectangle_object(data.registry, data.layer_id, pos, size);
  _verify_new_object(data, id, ObjectType::Rect, pos, size);
}

TEST(ObjectSystem, NewEllipseObject)
{
  auto data = _new_test_data();

  const Vector2f pos{12.3f, -83.2f};
  const Vector2f size{942.1f, 4'218.7f};

  const auto id = sys::new_ellipse_object(data.registry, data.layer_id, pos, size);
  _verify_new_object(data, id, ObjectType::Ellipse, pos, size);
}

TEST(ObjectSystem, NewPointObject)
{
  auto data = _new_test_data();

  const Vector2f pos{12.3f, -83.2f};

  const auto id = sys::new_point_object(data.registry, data.layer_id, pos);
  _verify_new_object(data, id, ObjectType::Point, pos, {0, 0});
}

TEST(ObjectSystem, RemoveObject)
{
  auto data = _new_test_data();
  ASSERT_THROW(sys::remove_object(data.registry, 1), TactileError);

  const auto id = sys::new_point_object(data.registry, data.layer_id, {0, 0});
  const auto snapshot = sys::remove_object(data.registry, id);

  ASSERT_EQ(entt::entity{entt::null}, sys::find_object(data.registry, id));
  ASSERT_EQ(id, snapshot.object.id);
  ASSERT_EQ(data.layer_id, snapshot.layer);

  ASSERT_THROW(sys::remove_object(data.registry, id), TactileError);
}

TEST(ObjectSystem, RemoveAndRestoreObject)
{
  auto data = _new_test_data();

  const Vector2f pos{74.3f, 745.9f};
  const Vector2f size{83.1f, 23.6f};

  const auto id = sys::new_rectangle_object(data.registry, data.layer_id, pos, size);

  {
    const auto objectEntity = sys::get_object(data.registry, id);

    auto& object = checked_get<comp::Object>(data.registry, objectEntity);
    object.tag = "tag";
    object.visible = false;

    auto& context = checked_get<comp::Context>(data.registry, objectEntity);
    context.name = "tag";

    sys::add_property(data.registry, context, "int", 42);
  }

  {
    const auto snapshot = sys::remove_object(data.registry, id);
    ASSERT_EQ(entt::entity{entt::null}, sys::find_object(data.registry, id));
    sys::restore_object(data.registry, snapshot);
  }

  const auto objectEntity = sys::get_object(data.registry, id);

  const auto& object = checked_get<comp::Object>(data.registry, objectEntity);
  ASSERT_EQ(id, object.id);
  ASSERT_EQ(ObjectType::Rect, object.type);
  ASSERT_EQ(pos, object.pos);
  ASSERT_EQ(size, object.size);
  ASSERT_EQ("tag", object.tag);
  ASSERT_FALSE(object.visible);

  const auto& context = checked_get<comp::Context>(data.registry, objectEntity);

  ASSERT_EQ(42, sys::get_property(data.registry, context, "int").value);
}