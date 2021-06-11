#include "object_layer.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(ObjectLayer, AddPoint)
{
  core::ObjectLayer layer;

  const auto id = 42_obj;
  EXPECT_FALSE(layer.HasObject(id));

  const auto x = 123;
  const auto y = 456;
  layer.AddPoint(id, x, y);
  ASSERT_TRUE(layer.HasObject(id));

  auto& point = layer.GetObject(id);
  EXPECT_EQ(core::ObjectType::Point, point.Type());
  EXPECT_EQ(x, point.X());
  EXPECT_EQ(y, point.Y());
}

TEST(ObjectLayer, AddRectangle)
{
  core::ObjectLayer layer;

  const auto id = 12_obj;
  EXPECT_FALSE(layer.HasObject(id));

  const auto x = 123;
  const auto y = 456;
  const auto width = 854;
  const auto height = 249;
  layer.AddRectangle(id, x, y, width, height);
  ASSERT_TRUE(layer.HasObject(id));

  auto& rect = layer.GetObject(id);
  EXPECT_EQ(core::ObjectType::Rectangle, rect.Type());
  EXPECT_EQ(x, rect.X());
  EXPECT_EQ(y, rect.Y());
  EXPECT_EQ(width, rect.Width());
  EXPECT_EQ(height, rect.Height());
}

TEST(ObjectLayer, RemoveObject)
{
  core::ObjectLayer layer;
  EXPECT_EQ(0, layer.ObjectCount());

  const auto id = 7_obj;

  layer.AddPoint(id, 123, 456);
  ASSERT_TRUE(layer.HasObject(id));
  EXPECT_EQ(1, layer.ObjectCount());

  layer.RemoveObject(id);
  EXPECT_FALSE(layer.HasObject(id));
  EXPECT_EQ(0, layer.ObjectCount());
}

TEST(ObjectLayer, HasObject)
{
  core::ObjectLayer layer;

  const auto id = 27_obj;
  EXPECT_FALSE(layer.HasObject(id));

  layer.AddPoint(id, 0, 0);
  EXPECT_TRUE(layer.HasObject(id));
}

TEST(ObjectLayer, GetObject)
{
  core::ObjectLayer layer;
  const auto& cLayer = layer;

  EXPECT_ANY_THROW(layer.GetObject(1_obj));
  EXPECT_ANY_THROW(cLayer.GetObject(1_obj));

  layer.AddPoint(7_obj, 12, 42);
  EXPECT_NO_THROW(layer.GetObject(7_obj));
  EXPECT_NO_THROW(cLayer.GetObject(7_obj));
}