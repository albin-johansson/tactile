// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/object_layer.hpp"

#include <gtest/gtest.h>

using namespace tactile;
using namespace tactile::core;

/// \tests tactile::ObjectLayer::add_object
TEST(ObjectLayer, AddObject)
{
  ObjectLayer layer {};
  EXPECT_EQ(layer.object_count(), 0);

  EXPECT_TRUE(layer.add_object(Object {ObjectType::kRect}).has_value());
  EXPECT_EQ(layer.object_count(), 1);

  EXPECT_TRUE(layer.add_object(Object {ObjectType::kEllipse}).has_value());
  EXPECT_EQ(layer.object_count(), 2);
}

/// \tests tactile::ObjectLayer::add_object
TEST(ObjectLayer, AddObjectsWithClashingPersistentIdentifiers)
{
  Object point {ObjectType::kPoint};
  point.set_persistent_id(42);

  Object ellipse {ObjectType::kEllipse};
  ellipse.set_persistent_id(42);

  ObjectLayer layer {};
  EXPECT_TRUE(layer.add_object(std::move(point)).has_value());
  EXPECT_FALSE(layer.add_object(std::move(ellipse)).has_value());

  EXPECT_EQ(layer.object_count(), 1);
}

/// \tests tactile::ObjectLayer::emplace_object
TEST(ObjectLayer, EmplaceObject)
{
  ObjectLayer layer {};
  EXPECT_EQ(layer.object_count(), 0);

  const auto& point = layer.emplace_object(ObjectType::kPoint);
  EXPECT_EQ(point.get_type(), ObjectType::kPoint);

  const auto& rect = layer.emplace_object(ObjectType::kRect);
  EXPECT_EQ(rect.get_type(), ObjectType::kRect);

  const auto& ellipse = layer.emplace_object(ObjectType::kEllipse);
  EXPECT_EQ(ellipse.get_type(), ObjectType::kEllipse);

  EXPECT_EQ(layer.object_count(), 3);
}

/// \tests tactile::ObjectLayer::remove_object
/// \tests tactile::ObjectLayer::has_object
TEST(ObjectLayer, RemoveObject)
{
  ObjectLayer layer {};
  auto& point = layer.emplace_object(ObjectType::kPoint);
  auto& rect = layer.emplace_object(ObjectType::kRect);

  const auto point_uuid = point.meta().uuid();
  const auto rect_uuid = rect.meta().uuid();

  ASSERT_EQ(layer.object_count(), 2);
  EXPECT_TRUE(layer.has_object(point_uuid));
  EXPECT_TRUE(layer.has_object(rect.meta().uuid()));

  EXPECT_TRUE(layer.remove_object(point_uuid).has_value());
  EXPECT_EQ(layer.object_count(), 1);
  EXPECT_FALSE(layer.has_object(point_uuid));
  EXPECT_TRUE(layer.has_object(rect_uuid));

  EXPECT_TRUE(layer.remove_object(rect_uuid).has_value());
  EXPECT_EQ(layer.object_count(), 0);
  EXPECT_FALSE(layer.has_object(point_uuid));
  EXPECT_FALSE(layer.has_object(rect_uuid));

  EXPECT_FALSE(layer.remove_object(point_uuid).has_value());
  EXPECT_FALSE(layer.remove_object(rect_uuid).has_value());
}

/// \tests tactile::ObjectLayer::find_object
TEST(ObjectLayer, FindObject)
{
  const auto dummy_uuid = UUID::generate();

  ObjectLayer layer {};
  const auto& const_layer = layer;

  EXPECT_EQ(layer.find_object(dummy_uuid), nullptr);
  EXPECT_EQ(const_layer.find_object(dummy_uuid), nullptr);

  auto& ellipse = layer.emplace_object(ObjectType::kEllipse);
  EXPECT_EQ(layer.find_object(ellipse.meta().uuid()), &ellipse);
  EXPECT_EQ(const_layer.find_object(ellipse.meta().uuid()), &ellipse);
}
