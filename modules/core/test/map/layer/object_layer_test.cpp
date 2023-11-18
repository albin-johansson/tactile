// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/layer/object_layer.hpp"

#include <gtest/gtest.h>

using namespace tactile;
using tactile::int_literals::operator""_uz;

/// \tests tactile::ObjectLayer::add_object
TEST(ObjectLayer, AddObject)
{
  const ObjectID object_id {42};

  const auto object1 = make_shared<Object>(ObjectType::kRect);
  const auto object2 = make_shared<Object>(ObjectType::kEllipse);

  ObjectLayer layer;
  EXPECT_EQ(layer.object_count(), 0_uz);

  layer.add_object(object_id, object1);
  EXPECT_EQ(layer.object_count(), 1_uz);
  EXPECT_EQ(layer.get_object(object_id), object1);

  layer.add_object(object_id, object2);
  EXPECT_EQ(layer.object_count(), 1_uz);
  EXPECT_EQ(layer.get_object(object_id), object2);
}

/// \tests tactile::ObjectLayer::remove_object
/// \tests tactile::ObjectLayer::has_object
TEST(ObjectLayer, RemoveObject)
{
  const ObjectID object1_id {212};
  const ObjectID object2_id {832};

  const auto object1 = make_shared<Object>(ObjectType::kPoint);
  const auto object2 = make_shared<Object>(ObjectType::kRect);

  ObjectLayer layer;
  layer.add_object(object1_id, object1);
  layer.add_object(object2_id, object2);
  ASSERT_EQ(layer.object_count(), 2_uz);
  EXPECT_TRUE(layer.has_object(object1_id));
  EXPECT_TRUE(layer.has_object(object2_id));

  EXPECT_EQ(layer.remove_object(object1_id), object1);
  EXPECT_EQ(layer.object_count(), 1_uz);
  EXPECT_FALSE(layer.has_object(object1_id));
  EXPECT_TRUE(layer.has_object(object2_id));

  EXPECT_EQ(layer.remove_object(object2_id), object2);
  EXPECT_EQ(layer.object_count(), 0_uz);
  EXPECT_FALSE(layer.has_object(object1_id));
  EXPECT_FALSE(layer.has_object(object2_id));

  EXPECT_EQ(layer.remove_object(object1_id), nullptr);
  EXPECT_EQ(layer.remove_object(object2_id), nullptr);
}

/// \tests tactile::ObjectLayer::get_object
TEST(ObjectLayer, GetObject)
{
  const ObjectID object_id {123};
  const auto object = make_shared<Object>(ObjectType::kPoint);

  ObjectLayer layer;
  EXPECT_ANY_THROW((void) layer.get_object(object_id));

  layer.add_object(object_id, object);
  EXPECT_EQ(layer.get_object(object_id), object);
}

/// \tests tactile::ObjectLayer::find_object
TEST(ObjectLayer, FindObject)
{
  const ObjectID object_id {923};
  const auto object = make_shared<Object>(ObjectType::kEllipse);

  ObjectLayer layer;
  const auto& const_layer = layer;

  EXPECT_EQ(layer.find_object(object_id), nullptr);
  EXPECT_EQ(const_layer.find_object(object_id), nullptr);

  layer.add_object(object_id, object);
  EXPECT_EQ(layer.find_object(object_id), object.get());
  EXPECT_EQ(const_layer.find_object(object_id), object.get());
}
