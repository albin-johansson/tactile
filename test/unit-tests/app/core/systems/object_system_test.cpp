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

#include "core/components/attributes.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/systems/registry_system.hpp"
#include "misc/throw.hpp"

using namespace tactile;

namespace {

struct TestData final
{
  entt::registry registry;
  LayerID layer_id{};
};

[[nodiscard]] auto _new_test_data() -> TestData
{
  TestData data{.registry = sys::make_document_registry()};

  const auto layerEntity = sys::new_object_layer(data.registry);
  const auto& layer = sys::checked_get<comp::Layer>(data.registry, layerEntity);

  data.layer_id = layer.id;

  return data;
}

/* Verifies the integrity of a newly created object */
void _verify_new_object(const TestData& data,
                        const ObjectID id,
                        const ObjectType type,
                        const float x,
                        const float y,
                        const float width,
                        const float height)
{
  const auto objectEntity = sys::get_object(data.registry, id);

  ASSERT_TRUE(data.registry.all_of<comp::Object>(objectEntity));
  ASSERT_TRUE(data.registry.all_of<comp::AttributeContext>(objectEntity));

  const auto& object = sys::checked_get<comp::Object>(data.registry, objectEntity);

  ASSERT_EQ(id, object.id);
  ASSERT_EQ(type, object.type);

  ASSERT_EQ(x, object.x);
  ASSERT_EQ(y, object.y);
  ASSERT_EQ(width, object.width);
  ASSERT_EQ(height, object.height);

  ASSERT_TRUE(object.visible);
  ASSERT_TRUE(object.tag.empty());
}

}  // namespace

TEST(ObjectSystem, NewRectangleObject)
{
  auto data = _new_test_data();

  const auto x = 84.3f;
  const auto y = 748.9f;
  const auto width = 432.6f;
  const auto height = 254.0f;

  const auto id =
      sys::new_rectangle_object(data.registry, data.layer_id, x, y, width, height);

  _verify_new_object(data, id, ObjectType::Rect, x, y, width, height);
}

TEST(ObjectSystem, NewEllipseObject)
{
  auto data = _new_test_data();

  const auto x = 12.3f;
  const auto y = -83.2f;
  const auto width = 942.1f;
  const auto height = 4'218.7f;

  const auto id =
      sys::new_ellipse_object(data.registry, data.layer_id, x, y, width, height);

  _verify_new_object(data, id, ObjectType::Ellipse, x, y, width, height);
}

TEST(ObjectSystem, NewPointObject)
{
  auto data = _new_test_data();

  const auto x = 12.3f;
  const auto y = -83.2f;

  const auto id = sys::new_point_object(data.registry, data.layer_id, x, y);
  _verify_new_object(data, id, ObjectType::Point, x, y, 0, 0);
}

TEST(ObjectSystem, RemoveObject)
{
  auto data = _new_test_data();
  ASSERT_THROW(sys::remove_object(data.registry, 1), TactileError);

  const auto id = sys::new_point_object(data.registry, data.layer_id, 0, 0);
  const auto snapshot = sys::remove_object(data.registry, id);

  ASSERT_EQ(entt::entity{entt::null}, sys::find_object(data.registry, id));
  ASSERT_EQ(id, snapshot.object.id);
  ASSERT_EQ(data.layer_id, snapshot.layer);

  ASSERT_THROW(sys::remove_object(data.registry, id), TactileError);
}

TEST(ObjectSystem, RemoveAndRestoreObject)
{
  auto data = _new_test_data();

  const auto x = 74.3f;
  const auto y = 745.9f;
  const auto width = 83.1f;
  const auto height = 23.6f;

  const auto id =
      sys::new_rectangle_object(data.registry, data.layer_id, x, y, width, height);

  {
    const auto objectEntity = sys::get_object(data.registry, id);

    auto& object = sys::checked_get<comp::Object>(data.registry, objectEntity);
    object.tag = "tag";
    object.visible = false;

    auto& context = sys::checked_get<comp::AttributeContext>(data.registry, objectEntity);
    context.name = "tag";

    sys::add_property(data.registry, context, "int", 42);
  }

  {
    const auto snapshot = sys::remove_object(data.registry, id);
    ASSERT_EQ(entt::entity{entt::null}, sys::find_object(data.registry, id));
    sys::restore_object(data.registry, snapshot);
  }

  const auto objectEntity = sys::get_object(data.registry, id);

  const auto& object = sys::checked_get<comp::Object>(data.registry, objectEntity);
  ASSERT_EQ(id, object.id);
  ASSERT_EQ(ObjectType::Rect, object.type);
  ASSERT_EQ(x, object.x);
  ASSERT_EQ(y, object.y);
  ASSERT_EQ(width, object.width);
  ASSERT_EQ(height, object.height);
  ASSERT_EQ("tag", object.tag);
  ASSERT_FALSE(object.visible);

  const auto& context =
      sys::checked_get<comp::AttributeContext>(data.registry, objectEntity);

  ASSERT_EQ(42, sys::get_property(data.registry, context, "int").value);
}