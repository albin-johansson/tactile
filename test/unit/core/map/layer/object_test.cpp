#include "object.hpp"

#include <gtest/gtest.h>

#include "tactile_qstring.hpp"

using namespace tactile;

TEST(Object, Defaults)
{
  const core::object rect{core::object_type::rectangle};
  EXPECT_TRUE(rect.is_rectangle());
  EXPECT_TRUE(rect.name().isEmpty());
  EXPECT_TRUE(rect.visible());
  EXPECT_FALSE(rect.is_point());
  EXPECT_FALSE(rect.custom_type());
  EXPECT_EQ(core::object_type::rectangle, rect.type());
  EXPECT_EQ(0, rect.x());
  EXPECT_EQ(0, rect.y());
  EXPECT_EQ(0, rect.width());
  EXPECT_EQ(0, rect.height());

  const core::object point{core::object_type::point};
  EXPECT_TRUE(point.is_point());
  EXPECT_TRUE(point.name().isEmpty());
  EXPECT_TRUE(point.visible());
  EXPECT_FALSE(point.is_rectangle());
  EXPECT_FALSE(point.custom_type());
  EXPECT_EQ(core::object_type::point, point.type());
  EXPECT_EQ(0, point.x());
  EXPECT_EQ(0, point.y());
  EXPECT_EQ(0, point.width());
  EXPECT_EQ(0, point.height());
}

TEST(Object, SetX)
{
  core::object object{core::object_type::point};

  const auto x = 12.3;
  object.set_x(x);

  EXPECT_EQ(x, object.x());
}

TEST(Object, SetY)
{
  core::object object{core::object_type::point};

  const auto y = 47.8;
  object.set_y(y);

  EXPECT_EQ(y, object.y());
}

TEST(Object, SetWidth)
{
  core::object object{core::object_type::rectangle};

  const auto width = 72.3;
  object.set_width(width);

  EXPECT_EQ(width, object.width());
}

TEST(Object, SetHeight)
{
  core::object object{core::object_type::rectangle};

  const auto height = 59.5;
  object.set_height(height);

  EXPECT_EQ(height, object.height());
}

TEST(Object, SetVisible)
{
  core::object object{core::object_type::point};

  object.set_visible(false);
  EXPECT_FALSE(object.visible());

  object.set_visible(true);
  EXPECT_TRUE(object.visible());
}

TEST(Object, SetName)
{
  core::object object{core::object_type::point};

  object.set_name(TACTILE_QSTRING(u"foo"));
  EXPECT_EQ(TACTILE_QSTRING(u"foo"), object.name());
}

TEST(Object, SetCustomType)
{
  core::object object{core::object_type::point};

  object.set_custom_type(TACTILE_QSTRING(u"foobar"));
  EXPECT_EQ(TACTILE_QSTRING(u"foobar"), object.custom_type());

  object.set_custom_type(std::nullopt);
  EXPECT_FALSE(object.custom_type());
}