#include "object.hpp"

#include <gtest/gtest.h>

#include "tactile_qstring.hpp"

using namespace tactile;

TEST(Object, Defaults)
{
  const core::Object rect{core::ObjectType::Rectangle};
  EXPECT_TRUE(rect.IsRectangle());
  EXPECT_TRUE(rect.Name().isEmpty());
  EXPECT_TRUE(rect.IsVisible());
  EXPECT_FALSE(rect.IsPoint());
  EXPECT_FALSE(rect.CustomType());
  EXPECT_EQ(core::ObjectType::Rectangle, rect.Type());
  EXPECT_EQ(0, rect.X());
  EXPECT_EQ(0, rect.Y());
  EXPECT_EQ(0, rect.Width());
  EXPECT_EQ(0, rect.Height());

  const core::Object point{core::ObjectType::Point};
  EXPECT_TRUE(point.IsPoint());
  EXPECT_TRUE(point.Name().isEmpty());
  EXPECT_TRUE(point.IsVisible());
  EXPECT_FALSE(point.IsRectangle());
  EXPECT_FALSE(point.CustomType());
  EXPECT_EQ(core::ObjectType::Point, point.Type());
  EXPECT_EQ(0, point.X());
  EXPECT_EQ(0, point.Y());
  EXPECT_EQ(0, point.Width());
  EXPECT_EQ(0, point.Height());
}

TEST(Object, SetX)
{
  core::Object object{core::ObjectType::Point};

  const auto x = 12.3;
  object.SetX(x);

  EXPECT_EQ(x, object.X());
}

TEST(Object, SetY)
{
  core::Object object{core::ObjectType::Point};

  const auto y = 47.8;
  object.SetY(y);

  EXPECT_EQ(y, object.Y());
}

TEST(Object, SetWidth)
{
  core::Object object{core::ObjectType::Rectangle};

  const auto width = 72.3;
  object.SetWidth(width);

  EXPECT_EQ(width, object.Width());
}

TEST(Object, SetHeight)
{
  core::Object object{core::ObjectType::Rectangle};

  const auto height = 59.5;
  object.SetHeight(height);

  EXPECT_EQ(height, object.Height());
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

  object.SetName(TACTILE_QSTRING(u"foo"));
  EXPECT_EQ(TACTILE_QSTRING(u"foo"), object.Name());
}

TEST(Object, SetCustomType)
{
  core::Object object{core::ObjectType::Point};

  object.SetCustomType(TACTILE_QSTRING(u"foobar"));
  EXPECT_EQ(TACTILE_QSTRING(u"foobar"), object.CustomType());

  object.SetCustomType(std::nullopt);
  EXPECT_FALSE(object.CustomType());
}