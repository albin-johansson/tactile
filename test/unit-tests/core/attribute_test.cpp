#include "core/attribute.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"

using namespace std::string_literals;

namespace tactile::test {

TEST(Attribute, Defaults)
{
  const Attribute value;
  ASSERT_EQ(AttributeType::String, value.type());

  ASSERT_TRUE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_path());
  ASSERT_FALSE(value.is_object());

  ASSERT_EQ("", value.as_string());
  ASSERT_THROW(value.as_int(), TactileError);
  ASSERT_THROW(value.as_float(), TactileError);
  ASSERT_THROW(value.as_bool(), TactileError);
  ASSERT_THROW(value.as_color(), TactileError);
  ASSERT_THROW(value.as_path(), TactileError);
  ASSERT_THROW(value.as_object(), TactileError);
}

TEST(Attribute, IntAttribute)
{
  const Attribute value {123};
  ASSERT_EQ(123, value.as_int());

  ASSERT_TRUE(value.is_int());
  ASSERT_TRUE(value.try_as_int());

  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_path());
  ASSERT_FALSE(value.is_object());
}

TEST(Attribute, FloatAttribute)
{
  const Attribute value {12.3f};
  ASSERT_EQ(12.3f, value.as_float());

  ASSERT_TRUE(value.is_float());
  ASSERT_TRUE(value.try_as_float());

  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_path());
  ASSERT_FALSE(value.is_object());
}

TEST(Attribute, StringAttribute)
{
  const Attribute value {"foo"s};
  ASSERT_EQ("foo", value.as_string());

  ASSERT_TRUE(value.is_string());
  ASSERT_TRUE(value.try_as_string());

  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_path());
  ASSERT_FALSE(value.is_object());
}

TEST(Attribute, BoolAttribute)
{
  const Attribute value {false};
  ASSERT_FALSE(value.as_bool());

  ASSERT_TRUE(value.is_bool());
  ASSERT_TRUE(value.try_as_bool());

  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_path());
  ASSERT_FALSE(value.is_object());
}

TEST(Attribute, FileAttribute)
{
  const std::filesystem::path file {"test-resources/foo.txt"};
  const Attribute             value {file};

  ASSERT_TRUE(value.is_path());
  ASSERT_TRUE(value.try_as_path());

  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_object());
}

TEST(Attribute, ObjectAttribute)
{
  const Attribute value {object_t {7}};

  ASSERT_TRUE(value.is_object());
  ASSERT_TRUE(value.try_as_object());

  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_color());
  ASSERT_FALSE(value.is_path());
}

TEST(Attribute, ColorAttribute)
{
  const Attribute value {cen::colors::red};

  ASSERT_TRUE(value.is_color());
  ASSERT_TRUE(value.try_as_color());

  ASSERT_FALSE(value.is_string());
  ASSERT_FALSE(value.is_int());
  ASSERT_FALSE(value.is_float());
  ASSERT_FALSE(value.is_bool());
  ASSERT_FALSE(value.is_path());
  ASSERT_FALSE(value.is_object());
}

TEST(Attribute, SetValue)
{
  Attribute value;

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

}  // namespace tactile::test
