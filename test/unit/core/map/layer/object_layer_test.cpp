#include "object_layer.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(ObjectLayer, AddPoint)
{
  core::object_layer layer;

  const auto id = 42_obj;
  EXPECT_FALSE(layer.has_object(id));

  const auto x = 123;
  const auto y = 456;
  layer.add_point(id, x, y);
  ASSERT_TRUE(layer.has_object(id));

  auto& point = layer.get_object(id);
  EXPECT_EQ(core::object_type::point, point.type());
  EXPECT_EQ(x, point.x());
  EXPECT_EQ(y, point.y());
}

TEST(ObjectLayer, AddRectangle)
{
  core::object_layer layer;

  const auto id = 12_obj;
  EXPECT_FALSE(layer.has_object(id));

  const auto x = 123;
  const auto y = 456;
  const auto width = 854;
  const auto height = 249;
  layer.add_rectangle(id, x, y, width, height);
  ASSERT_TRUE(layer.has_object(id));

  auto& rect = layer.get_object(id);
  EXPECT_EQ(core::object_type::rectangle, rect.type());
  EXPECT_EQ(x, rect.x());
  EXPECT_EQ(y, rect.y());
  EXPECT_EQ(width, rect.width());
  EXPECT_EQ(height, rect.height());
}

TEST(ObjectLayer, RemoveObject)
{
  core::object_layer layer;
  EXPECT_EQ(0, layer.object_count());

  const auto id = 7_obj;

  layer.add_point(id, 123, 456);
  ASSERT_TRUE(layer.has_object(id));
  EXPECT_EQ(1, layer.object_count());

  layer.remove_object(id);
  EXPECT_FALSE(layer.has_object(id));
  EXPECT_EQ(0, layer.object_count());
}

TEST(ObjectLayer, HasObject)
{
  core::object_layer layer;

  const auto id = 27_obj;
  EXPECT_FALSE(layer.has_object(id));

  layer.add_point(id, 0, 0);
  EXPECT_TRUE(layer.has_object(id));
}

TEST(ObjectLayer, GetObject)
{
  core::object_layer layer;
  const auto& cLayer = layer;

  EXPECT_ANY_THROW(layer.get_object(1_obj));
  EXPECT_ANY_THROW(cLayer.get_object(1_obj));

  layer.add_point(7_obj, 12, 42);
  EXPECT_NO_THROW(layer.get_object(7_obj));
  EXPECT_NO_THROW(cLayer.get_object(7_obj));
}