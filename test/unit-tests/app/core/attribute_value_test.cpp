#include "core/attribute.hpp"

#include <gtest/gtest.h>

#include "misc/throw.hpp"

using namespace tactile;
using namespace std::string_literals;

TEST(AttributeValue, Defaults)
{
  const attribute_value value;
  ASSERT_EQ(attribute_type::string, value.type());

  ASSERT_TRUE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_file());
  ASSERT_FALSE(value.is_object());

  ASSERT_EQ("", value.as_string());
  ASSERT_THROW(value.as_int(), tactile_error);
  ASSERT_THROW(value.as_float(), tactile_error);
  ASSERT_THROW(value.as_bool(), tactile_error);
  ASSERT_THROW(value.as_color(), tactile_error);
  ASSERT_THROW(value.as_file(), tactile_error);
  ASSERT_THROW(value.as_object(), tactile_error);
}

TEST(AttributeValue, IntAttribute)
{
  const attribute_value value{123};
  ASSERT_EQ(123, value.as_int());

  ASSERT_TRUE(value.is_int());
  ASSERT_TRUE(value.try_as_int());

  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_file());
  ASSERT_FALSE(value.is_object());
}

TEST(AttributeValue, FloatAttribute)
{
  const attribute_value value{12.3f};
  ASSERT_EQ(12.3f, value.as_float());

  ASSERT_TRUE(value.is_float());
  ASSERT_TRUE(value.try_as_float());

  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_file());
  ASSERT_FALSE(value.is_object());
}

TEST(AttributeValue, StringAttribute)
{
  const attribute_value value{"foo"s};
  ASSERT_EQ("foo", value.as_string());

  ASSERT_TRUE(value.is_string());
  ASSERT_TRUE(value.try_as_string());

  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_file());
  ASSERT_FALSE(value.is_object());
}

TEST(AttributeValue, BoolAttribute)
{
  const attribute_value value{false};
  ASSERT_FALSE(value.as_bool());

  ASSERT_TRUE(value.is_bool());
  ASSERT_TRUE(value.try_as_bool());

  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_file());
  ASSERT_FALSE(value.is_object());
}

TEST(AttributeValue, FileAttribute)
{
  const std::filesystem::path file{"test-resources/foo.txt"};
  const attribute_value value{file};

  ASSERT_TRUE(value.is_file());
  ASSERT_TRUE(value.try_as_file());

  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_object());
}

TEST(AttributeValue, ObjectAttribute)
{
  const attribute_value value{object_t{7}};

  ASSERT_TRUE(value.is_object());
  ASSERT_TRUE(value.try_as_object());

  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_file());
}

TEST(AttributeValue, ColorAttribute)
{
  const attribute_value value{cen::colors::red};

  ASSERT_TRUE(value.is_color());
  ASSERT_TRUE(value.try_as_color());

  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_file());
  ASSERT_FALSE(value.is_object());
}

TEST(AttributeValue, SetValue)
{
  attribute_value value;

  value.set_value(10);
  ASSERT_TRUE(value.is_int());
  ASSERT_EQ(10, value.as_int());

  value.set_value(93.2f);
  ASSERT_TRUE(value.is_float());
  ASSERT_EQ(93.2f, value.as_float());

  value.set_value(true);
  ASSERT_TRUE(value.is_bool());
  ASSERT_TRUE(value.as_bool());

  value.set_value("foo"s);
  ASSERT_TRUE(value.is_string());
  ASSERT_EQ("foo", value.as_string());
}
