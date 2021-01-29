#include "property.hpp"

#include <gtest/gtest.h>

#include "tactile_qstring.hpp"

using namespace tactile;

TEST(Property, Defaults)
{
  const core::property property;
  EXPECT_FALSE(property.has_value());
  EXPECT_FALSE(property.get_type());

  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_integer());
  EXPECT_FALSE(property.is_floating());
  EXPECT_FALSE(property.is_boolean());
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_file());
  EXPECT_FALSE(property.is_object());
}

TEST(Property, IntProperty)
{
  const core::property property{123};
  ASSERT_TRUE(property.has_value());
  ASSERT_TRUE(property.is_integer());
  EXPECT_TRUE(property.try_as_integer());
  EXPECT_EQ(123, property.as_integer());

  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_floating());
  EXPECT_FALSE(property.is_boolean());
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_file());
  EXPECT_FALSE(property.is_object());
}

TEST(Property, FloatProperty)
{
  const core::property property{12.3};
  ASSERT_TRUE(property.has_value());
  ASSERT_TRUE(property.is_floating());
  EXPECT_TRUE(property.try_as_floating());
  EXPECT_EQ(12.3, property.as_floating());

  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_integer());
  EXPECT_FALSE(property.is_boolean());
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_file());
  EXPECT_FALSE(property.is_object());
}

TEST(Property, StringProperty)
{
  const core::property property{TACTILE_QSTRING(u"foo")};
  ASSERT_TRUE(property.has_value());
  ASSERT_TRUE(property.is_string());
  EXPECT_TRUE(property.try_as_string());
  EXPECT_EQ(TACTILE_QSTRING(u"foo"), property.as_string());

  EXPECT_FALSE(property.is_integer());
  EXPECT_FALSE(property.is_floating());
  EXPECT_FALSE(property.is_boolean());
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_file());
  EXPECT_FALSE(property.is_object());
}

TEST(Property, BoolProperty)
{
  const core::property property{false};
  ASSERT_TRUE(property.has_value());
  ASSERT_TRUE(property.is_boolean());
  EXPECT_TRUE(property.try_as_boolean());
  EXPECT_FALSE(property.as_boolean());

  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_integer());
  EXPECT_FALSE(property.is_floating());
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_file());
  EXPECT_FALSE(property.is_object());
}

TEST(Property, FileProperty)
{
  const QFileInfo file{TACTILE_QSTRING(u"resources/foo.txt")};
  const core::property property{file};
  ASSERT_TRUE(property.has_value());
  ASSERT_TRUE(property.is_file());
  EXPECT_TRUE(property.try_as_file());

  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_integer());
  EXPECT_FALSE(property.is_floating());
  EXPECT_FALSE(property.is_boolean());
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_object());
}

TEST(Property, ObjectProperty)
{
  const core::property property{core::object_ref{7}};
  ASSERT_TRUE(property.has_value());
  ASSERT_TRUE(property.is_object());
  EXPECT_TRUE(property.try_as_object());

  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_integer());
  EXPECT_FALSE(property.is_floating());
  EXPECT_FALSE(property.is_boolean());
  EXPECT_FALSE(property.is_color());
  EXPECT_FALSE(property.is_file());
}

TEST(Property, ColorProperty)
{
  const QColor color{Qt::red};
  const core::property property{color};
  ASSERT_TRUE(property.has_value());
  ASSERT_TRUE(property.is_color());
  EXPECT_TRUE(property.try_as_color());

  EXPECT_FALSE(property.is_string());
  EXPECT_FALSE(property.is_integer());
  EXPECT_FALSE(property.is_floating());
  EXPECT_FALSE(property.is_boolean());
  EXPECT_FALSE(property.is_file());
  EXPECT_FALSE(property.is_object());
}

TEST(Property, Reset)
{
  core::property property;
  ASSERT_FALSE(property.has_value());

  property.set_value(123);
  EXPECT_TRUE(property.has_value());

  property.reset();
  EXPECT_FALSE(property.has_value());

  EXPECT_NO_THROW(property.reset());
  EXPECT_NO_THROW(property.reset());
}

TEST(Property, SetValue)
{
  core::property property;

  property.set_value(10);
  ASSERT_TRUE(property.is_integer());
  EXPECT_EQ(10, property.as_integer());

  property.set_value(93.2);
  ASSERT_TRUE(property.is_floating());
  EXPECT_EQ(93.2, property.as_floating());

  property.set_value(true);
  ASSERT_TRUE(property.is_boolean());
  EXPECT_TRUE(property.as_boolean());

  property.set_value(TACTILE_QSTRING(u"foo"));
  ASSERT_TRUE(property.is_string());
  EXPECT_EQ(TACTILE_QSTRING(u"foo"), property.as_string());
}

TEST(Property, HasValue)
{
  core::property property;
  EXPECT_FALSE(property.has_value());

  property.set_value(924);
  EXPECT_TRUE(property.has_value());
}

TEST(Property, As)
{
  const core::property property{123};

  EXPECT_NO_THROW(property.as_integer());
  EXPECT_ANY_THROW(property.as_floating());
  EXPECT_ANY_THROW(property.as_string());
  EXPECT_ANY_THROW(property.as_boolean());
  EXPECT_ANY_THROW(property.as_object());
  EXPECT_ANY_THROW(property.as_file());
  EXPECT_ANY_THROW(property.as_color());
}

TEST(Property, TryAs)
{
  core::property property;

  EXPECT_FALSE(property.try_as_integer());
  EXPECT_FALSE(property.try_as_floating());
  EXPECT_FALSE(property.try_as_string());
  EXPECT_FALSE(property.try_as_boolean());

  property.set_value(123);
  EXPECT_TRUE(property.try_as_integer());
  EXPECT_FALSE(property.try_as_floating());
  EXPECT_FALSE(property.try_as_string());
  EXPECT_FALSE(property.try_as_boolean());
}

TEST(Property, GetType)
{
  core::property property;
  EXPECT_FALSE(property.get_type());

  property.set_value(123);
  EXPECT_EQ(core::property::integer, property.get_type());

  property.set_value(12.3);
  EXPECT_EQ(core::property::floating, property.get_type());

  property.set_value(TACTILE_QSTRING(u"foo"));
  EXPECT_EQ(core::property::string, property.get_type());

  property.reset();
  EXPECT_FALSE(property.get_type());
}
