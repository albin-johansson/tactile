#include "core/map/layers/object.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(Object, Defaults)
{
  const Object rect{ObjectType::Rectangle};
  ASSERT_EQ(0, rect.GetX());
  ASSERT_EQ(0, rect.GetY());
  ASSERT_EQ(0, rect.GetWidth());
  ASSERT_EQ(0, rect.GetHeight());
  ASSERT_EQ(ObjectType::Rectangle, rect.GetType());
  ASSERT_TRUE(rect.IsRectangle());
  ASSERT_FALSE(rect.IsPoint());
  ASSERT_TRUE(rect.IsVisible());
  ASSERT_TRUE(rect.GetName().empty());
  ASSERT_FALSE(rect.GetCustomType());

  const Object point{ObjectType::Point};
  ASSERT_EQ(0, point.GetX());
  ASSERT_EQ(0, point.GetY());
  ASSERT_EQ(0, point.GetWidth());
  ASSERT_EQ(0, point.GetHeight());
  ASSERT_EQ(ObjectType::Point, point.GetType());
  ASSERT_FALSE(point.IsRectangle());
  ASSERT_TRUE(point.IsPoint());
  ASSERT_TRUE(point.IsVisible());
  ASSERT_TRUE(point.GetName().empty());
  ASSERT_FALSE(point.GetCustomType());
}

TEST(Object, SetX)
{
  Object object{ObjectType::Point};

  const auto x = 12.3f;
  object.SetX(x);

  ASSERT_EQ(x, object.GetX());
}

TEST(Object, SetY)
{
  Object object{ObjectType::Point};

  const auto y = 47.8f;
  object.SetY(y);

  ASSERT_EQ(y, object.GetY());
}

TEST(Object, SetWidth)
{
  Object object{ObjectType::Rectangle};

  const auto width = 72.3f;
  object.SetWidth(width);

  ASSERT_EQ(width, object.GetWidth());
}

TEST(Object, SetHeight)
{
  Object object{ObjectType::Rectangle};

  const auto height = 59.5f;
  object.SetHeight(height);

  ASSERT_EQ(height, object.GetHeight());
}

TEST(Object, SetVisible)
{
  Object object{ObjectType::Point};

  object.SetVisible(false);
  ASSERT_FALSE(object.IsVisible());

  object.SetVisible(true);
  ASSERT_TRUE(object.IsVisible());
}

TEST(Object, SetName)
{
  Object object{ObjectType::Point};

  object.SetName("foo");
  ASSERT_EQ("foo", object.GetName());
}

TEST(Object, SetCustomType)
{
  Object object{ObjectType::Point};

  object.SetCustomType("foobar");
  ASSERT_EQ("foobar", object.GetCustomType());

  object.SetCustomType(nothing);
  ASSERT_FALSE(object.GetCustomType());
}
