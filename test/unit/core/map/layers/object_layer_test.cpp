#include "core/map/layers/object_layer.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(ObjectLayer, Defaults)
{
  const ObjectLayer layer;
  ASSERT_EQ(0, layer.GetObjectCount());
  ASSERT_EQ(LayerType::ObjectLayer, layer.GetType());
}

TEST(ObjectLayer, Each)
{
  ObjectLayer layer;

  int count = 0;

  layer.Each([&](const object_id id, const Object&) { ++count; });
  ASSERT_EQ(0, count);

  layer.AddPoint(42_obj, 100, 100);
  layer.AddPoint(27_obj, 27, 123);
  layer.AddRectangle(82_obj, 47, 63, 123, 103);

  layer.Each([&](const object_id id, const Object&) { ++count; });
  ASSERT_EQ(3, count);
}

TEST(ObjectLayer, AddObject)
{
  ObjectLayer layer;
  ASSERT_FALSE(layer.HasObject(42_obj));

  layer.AddObject(42_obj, Object{ObjectType::Point});
  ASSERT_TRUE(layer.HasObject(42_obj));
}

TEST(ObjectLayer, AddPoint)
{
  ObjectLayer layer;

  const auto x = 12.3f;
  const auto y = 74.8f;
  layer.AddPoint(10_obj, x, y);
  ASSERT_TRUE(layer.HasObject(10_obj));

  const auto& point = layer.GetObject(10_obj);
  ASSERT_EQ(x, point.GetX());
  ASSERT_EQ(y, point.GetY());
  ASSERT_EQ(0, point.GetWidth());
  ASSERT_EQ(0, point.GetHeight());
  ASSERT_EQ(ObjectType::Point, point.GetType());
  ASSERT_TRUE(point.IsPoint());
}

TEST(ObjectLayer, AddRectangle)
{
  ObjectLayer layer;

  const auto x = 12.3f;
  const auto y = 74.8f;
  const auto width = 123.9f;
  const auto height = 83.2f;
  layer.AddRectangle(12_obj, x, y, width, height);
  ASSERT_TRUE(layer.HasObject(12_obj));

  const auto& rect = layer.GetObject(12_obj);
  ASSERT_EQ(x, rect.GetX());
  ASSERT_EQ(y, rect.GetY());
  ASSERT_EQ(width, rect.GetWidth());
  ASSERT_EQ(height, rect.GetHeight());
  ASSERT_EQ(ObjectType::Rectangle, rect.GetType());
  ASSERT_TRUE(rect.IsRectangle());
}

TEST(ObjectLayer, RemoveObject)
{
  ObjectLayer layer;
  ASSERT_EQ(0, layer.GetObjectCount());

  layer.AddPoint(7_obj, 42, 27);
  ASSERT_EQ(1, layer.GetObjectCount());

  layer.RemoveObject(7_obj);
  ASSERT_EQ(0, layer.GetObjectCount());
}

TEST(ObjectLayer, GetObject)
{
  ObjectLayer layer;
  ASSERT_ANY_THROW(layer.GetObject(42_obj));

  layer.AddPoint(42_obj, 42, 55);
  ASSERT_NO_THROW(layer.GetObject(42_obj));

  const auto& cLayer = layer;
  ASSERT_NO_THROW(cLayer.GetObject(42_obj));
}
