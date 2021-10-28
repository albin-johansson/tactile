#include <tactile-base/property_value.hpp>

#include <gtest/gtest.h>

using namespace Tactile;
using namespace std::string_literals;

TEST(PropertyValue, Defaults)
{
  const PropertyValue property;
  ASSERT_FALSE(property.HasValue());
  ASSERT_FALSE(property.GetType());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsInt());
  ASSERT_FALSE(property.IsFloat());
  ASSERT_FALSE(property.IsBool());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsFile());
  ASSERT_FALSE(property.IsObject());
}

TEST(PropertyValue, IntProperty)
{
  const PropertyValue property{123};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsInt());
  ASSERT_TRUE(property.TryAsInt());
  ASSERT_EQ(123, property.AsInt());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsFloat());
  ASSERT_FALSE(property.IsBool());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsFile());
  ASSERT_FALSE(property.IsObject());
}

TEST(PropertyValue, FloatProperty)
{
  const PropertyValue property{12.3f};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsFloat());
  ASSERT_TRUE(property.TryAsFloat());
  ASSERT_EQ(12.3f, property.AsFloat());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsInt());
  ASSERT_FALSE(property.IsBool());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsFile());
  ASSERT_FALSE(property.IsObject());
}

TEST(PropertyValue, StringProperty)
{
  const PropertyValue property{"foo"s};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsString());
  ASSERT_TRUE(property.TryAsString());
  ASSERT_EQ("foo", property.AsString());

  ASSERT_FALSE(property.IsInt());
  ASSERT_FALSE(property.IsFloat());
  ASSERT_FALSE(property.IsBool());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsFile());
  ASSERT_FALSE(property.IsObject());
}

TEST(PropertyValue, BoolProperty)
{
  const PropertyValue property{false};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsBool());
  ASSERT_TRUE(property.TryAsBool());
  ASSERT_FALSE(property.AsBool());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsInt());
  ASSERT_FALSE(property.IsFloat());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsFile());
  ASSERT_FALSE(property.IsObject());
}

TEST(PropertyValue, FileProperty)
{
  const std::filesystem::path file{"resources/foo.txt"};
  const PropertyValue property{file};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsFile());
  ASSERT_TRUE(property.TryAsFile());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsInt());
  ASSERT_FALSE(property.IsFloat());
  ASSERT_FALSE(property.IsBool());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsObject());
}

TEST(PropertyValue, ObjectProperty)
{
  const PropertyValue property{ObjectRef{7}};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsObject());
  ASSERT_TRUE(property.TryAsObject());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsInt());
  ASSERT_FALSE(property.IsFloat());
  ASSERT_FALSE(property.IsBool());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsFile());
}

TEST(PropertyValue, ColorProperty)
{
  const auto color = cen::colors::red;
  const PropertyValue property{color};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsColor());
  ASSERT_TRUE(property.TryAsColor());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsInt());
  ASSERT_FALSE(property.IsFloat());
  ASSERT_FALSE(property.IsBool());
  ASSERT_FALSE(property.IsFile());
  ASSERT_FALSE(property.IsObject());
}

TEST(PropertyValue, Reset)
{
  PropertyValue property;
  ASSERT_FALSE(property.HasValue());

  property.SetValue(123);
  ASSERT_TRUE(property.HasValue());

  property.Reset();
  ASSERT_FALSE(property.HasValue());

  ASSERT_NO_THROW(property.Reset());
  ASSERT_NO_THROW(property.Reset());
}

TEST(PropertyValue, SetValue)
{
  PropertyValue property;

  property.SetValue(10);
  ASSERT_TRUE(property.IsInt());
  ASSERT_EQ(10, property.AsInt());

  property.SetValue(93.2f);
  ASSERT_TRUE(property.IsFloat());
  ASSERT_EQ(93.2f, property.AsFloat());

  property.SetValue(true);
  ASSERT_TRUE(property.IsBool());
  ASSERT_TRUE(property.AsBool());

  property.SetValue("foo"s);
  ASSERT_TRUE(property.IsString());
  ASSERT_EQ("foo", property.AsString());
}

TEST(PropertyValue, HasValue)
{
  PropertyValue property;
  ASSERT_FALSE(property.HasValue());

  property.SetValue(924);
  ASSERT_TRUE(property.HasValue());
}

TEST(PropertyValue, As)
{
  const PropertyValue property{123};

  ASSERT_NO_THROW(property.AsInt());
  ASSERT_ANY_THROW(property.AsFloat());
  ASSERT_ANY_THROW(property.AsString());
  ASSERT_ANY_THROW(property.AsBool());
  ASSERT_ANY_THROW(property.AsObject());
  ASSERT_ANY_THROW(property.AsFile());
  ASSERT_ANY_THROW(property.AsColor());
}

TEST(PropertyValue, TryAs)
{
  PropertyValue property;

  ASSERT_FALSE(property.TryAsInt());
  ASSERT_FALSE(property.TryAsFloat());
  ASSERT_FALSE(property.TryAsString());
  ASSERT_FALSE(property.TryAsBool());

  property.SetValue(123);
  ASSERT_TRUE(property.TryAsInt());
  ASSERT_FALSE(property.TryAsFloat());
  ASSERT_FALSE(property.TryAsString());
  ASSERT_FALSE(property.TryAsBool());
}

TEST(PropertyValue, GetType)
{
  PropertyValue property;
  ASSERT_FALSE(property.GetType());

  property.SetValue(123);
  ASSERT_EQ(PropertyType::Integer, property.GetType());

  property.SetValue(12.3f);
  ASSERT_EQ(PropertyType::Floating, property.GetType());

  property.SetValue("foo"s);
  ASSERT_EQ(PropertyType::String, property.GetType());

  property.Reset();
  ASSERT_FALSE(property.GetType());
}
