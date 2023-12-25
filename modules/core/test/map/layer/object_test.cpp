// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/layer/object.hpp"

#include <gtest/gtest.h>

#include "testutil/meta_context_helpers.hpp"

using namespace tactile;

TEST(Object, Defaults)
{
  const Object object {ObjectType::kPoint};
  EXPECT_EQ(object.get_position(), Float2(0.0f, 0.0f));
  EXPECT_EQ(object.get_size(), Float2(0.0f, 0.0f));
  EXPECT_FALSE(object.get_persistent_id().has_value());
  EXPECT_TRUE(object.meta().name().empty());
  EXPECT_TRUE(object.get_tag().empty());
  EXPECT_TRUE(object.is_visible());
}

TEST(Object, AcceptMetaContextVisitor)
{
  testutil::MetaContextVisitorTester visitor {};

  Object object {ObjectType::kEllipse};
  object.accept(visitor);

  EXPECT_EQ(visitor.orthogonal_map_count, 0);
  EXPECT_EQ(visitor.isometric_map_count, 0);
  EXPECT_EQ(visitor.hexagonal_map_count, 0);
  EXPECT_EQ(visitor.group_layer_count, 0);
  EXPECT_EQ(visitor.tile_layer_count, 0);
  EXPECT_EQ(visitor.object_layer_count, 0);
  EXPECT_EQ(visitor.object_count, 1);
  EXPECT_EQ(visitor.tileset_count, 0);
  EXPECT_EQ(visitor.tile_count, 0);
}

/// \tests tactile::Object::set_type
/// \tests tactile::Object::get_type
TEST(Object, SetType)
{
  Object object {ObjectType::kEllipse};

  object.set_type(ObjectType::kPoint);
  EXPECT_EQ(object.get_type(), ObjectType::kPoint);

  object.set_type(ObjectType::kRect);
  EXPECT_EQ(object.get_type(), ObjectType::kRect);

  object.set_type(ObjectType::kEllipse);
  EXPECT_EQ(object.get_type(), ObjectType::kEllipse);
}

/// \tests tactile::Object::set_position
/// \tests tactile::Object::get_position
TEST(Object, SetPosition)
{
  Object object {ObjectType::kEllipse};

  const Float2 position {1.0f, -1.0f};
  object.set_position(position);

  EXPECT_EQ(object.get_position(), position);
}

/// \tests tactile::Object::set_size
/// \tests tactile::Object::get_size
TEST(Object, SetSize)
{
  Object object {ObjectType::kRect};

  const Float2 size {81.4f, 938.1f};
  object.set_size(size);

  EXPECT_EQ(object.get_size(), size);
}

/// \tests tactile::Object::set_tag
/// \tests tactile::Object::get_tag
TEST(Object, SetTag)
{
  Object object {ObjectType::kPoint};

  const String tag {"foobar"};
  object.set_tag(tag);

  EXPECT_EQ(object.get_tag(), tag);
}

/// \tests tactile::Object::set_persistent_id
/// \tests tactile::Object::get_persistent_id
TEST(Object, SetPersistentId)
{
  Object object {ObjectType::kEllipse};

  object.set_persistent_id(42);
  EXPECT_EQ(object.get_persistent_id(), 42);

  object.set_persistent_id(kNothing);
  EXPECT_FALSE(object.get_persistent_id().has_value());
}

/// \tests tactile::Object::set_visible
/// \tests tactile::Object::is_visible
TEST(Object, SetVisible)
{
  Object object {ObjectType::kPoint};

  object.set_visible(false);
  EXPECT_FALSE(object.is_visible());

  object.set_visible(true);
  EXPECT_TRUE(object.is_visible());
}
