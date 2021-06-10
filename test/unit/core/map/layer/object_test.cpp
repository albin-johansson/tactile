#include "object.hpp"

#include <gtest/gtest.h>

#include "tactile_qstring.hpp"

using namespace tactile;

TEST(Object, Defaults)
{
  const core::Object rect{core::object_type::rectangle};
  EXPECT_TRUE(rect.IsRectangle());
  EXPECT_TRUE(rect.name().isEmpty());
  EXPECT_TRUE(rect.visible());
  EXPECT_FALSE(rect.IsPoint());
  EXPECT_FALSE(rect.custom_type());
  EXPECT_EQ(core::object_type::rectangle, rect.type());
  EXPECT_EQ(0, rect.x());
  EXPECT_EQ(0, rect.y());
  EXPECT_EQ(0, rect.width());
  EXPECT_EQ(0, rect.height());

  const core::Object point{core::object_type::point};
  EXPECT_TRUE(point.IsPoint());
  EXPECT_TRUE(point.name().isEmpty());
  EXPECT_TRUE(point.visible());
  EXPECT_FALSE(point.IsRectangle());
  EXPECT_FALSE(point.custom_type());
  EXPECT_EQ(core::object_type::point, point.type());
  EXPECT_EQ(0, point.x());
  EXPECT_EQ(0, point.y());
  EXPECT_EQ(0, point.width());
  EXPECT_EQ(0, point.height());
}

TEST(Object, SetX)
{
  core::Object object{core::object_type::point};

  const auto x = 12.3;
  object.SetX(x);

  EXPECT_EQ(x, object.x());
}

TEST(Object, SetY)
{
  core::Object object{core::object_type::point};

  const auto y = 47.8;
  object.SetY(y);

  EXPECT_EQ(y, object.y());
}

TEST(Object, SetWidth)
{
  core::Object object{core::object_type::rectangle};

  const auto width = 72.3;
  object.SetWidth(width);

  EXPECT_EQ(width, object.width());
}

TEST(Object, SetHeight)
{
  core::Object object{core::object_type::rectangle};

  const auto height = 59.5;
  object.SetHeight(height);

  EXPECT_EQ(height, object.height());
}

TEST(Object, SetVisible)
{
  core::Object object{core::object_type::point};

  object.SetVisible(false);
  EXPECT_FALSE(object.visible());

  object.SetVisible(true);
  EXPECT_TRUE(object.visible());
}

TEST(Object, SetName)
{
  core::Object object{core::object_type::point};

  object.SetName(TACTILE_QSTRING(u"foo"));
  EXPECT_EQ(TACTILE_QSTRING(u"foo"), object.name());
}

TEST(Object, SetCustomType)
{
  core::Object object{core::object_type::point};

  object.SetCustomType(TACTILE_QSTRING(u"foobar"));
  EXPECT_EQ(TACTILE_QSTRING(u"foobar"), object.custom_type());

  object.SetCustomType(std::nullopt);
  EXPECT_FALSE(object.custom_type());
}