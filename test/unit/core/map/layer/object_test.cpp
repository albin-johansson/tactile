#include "object.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(Object, Defaults)
{
  const core::Object rect{core::ObjectType::Rectangle};
  EXPECT_TRUE(rect.IsRectangle());
  EXPECT_TRUE(rect.GetName().isEmpty());
  EXPECT_TRUE(rect.IsVisible());
  EXPECT_FALSE(rect.IsPoint());
  EXPECT_FALSE(rect.GetCustomType());
  EXPECT_EQ(core::ObjectType::Rectangle, rect.GetType());
  EXPECT_EQ(0, rect.GetX());
  EXPECT_EQ(0, rect.GetY());
  EXPECT_EQ(0, rect.GetWidth());
  EXPECT_EQ(0, rect.GetHeight());

  const core::Object point{core::ObjectType::Point};
  EXPECT_TRUE(point.IsPoint());
  EXPECT_TRUE(point.GetName().isEmpty());
  EXPECT_TRUE(point.IsVisible());
  EXPECT_FALSE(point.IsRectangle());
  EXPECT_FALSE(point.GetCustomType());
  EXPECT_EQ(core::ObjectType::Point, point.GetType());
  EXPECT_EQ(0, point.GetX());
  EXPECT_EQ(0, point.GetY());
  EXPECT_EQ(0, point.GetWidth());
  EXPECT_EQ(0, point.GetHeight());
}

TEST(Object, SetX)
{
  core::Object object{core::ObjectType::Point};

  const auto x = 12.3;
  object.SetX(x);

  EXPECT_EQ(x, object.GetX());
}

TEST(Object, SetY)
{
  core::Object object{core::ObjectType::Point};

  const auto y = 47.8;
  object.SetY(y);

  EXPECT_EQ(y, object.GetY());
}

TEST(Object, SetWidth)
{
  core::Object object{core::ObjectType::Rectangle};

  const auto width = 72.3;
  object.SetWidth(width);

  EXPECT_EQ(width, object.GetWidth());
}

TEST(Object, SetHeight)
{
  core::Object object{core::ObjectType::Rectangle};

  const auto height = 59.5;
  object.SetHeight(height);

  EXPECT_EQ(height, object.GetHeight());
}

TEST(Object, SetVisible)
{
  core::Object object{core::ObjectType::Point};

  object.SetVisible(false);
  EXPECT_FALSE(object.IsVisible());

  object.SetVisible(true);
  EXPECT_TRUE(object.IsVisible());
}

TEST(Object, SetName)
{
  core::Object object{core::ObjectType::Point};

  object.SetName(QStringLiteral(u"foo"));
  EXPECT_EQ(QStringLiteral(u"foo"), object.GetName());
}

TEST(Object, SetCustomType)
{
  core::Object object{core::ObjectType::Point};

  object.SetCustomType(QStringLiteral(u"foobar"));
  EXPECT_EQ(QStringLiteral(u"foobar"), object.GetCustomType());

  object.SetCustomType(std::nullopt);
  EXPECT_FALSE(object.GetCustomType());
}