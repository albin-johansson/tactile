#include "core/properties/property.hpp"

#include <gtest/gtest.h>

using namespace Tactile;
using namespace std::string_literals;

TEST(Property, Defaults)
{
  const Property property;
  ASSERT_FALSE(property.HasValue());
  ASSERT_FALSE(property.GetType());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsInteger());
  ASSERT_FALSE(property.IsFloating());
  ASSERT_FALSE(property.IsBoolean());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsFile());
  ASSERT_FALSE(property.IsObject());
}

TEST(Property, IntProperty)
{
  const Property property{123};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsInteger());
  ASSERT_TRUE(property.TryAsInteger());
  ASSERT_EQ(123, property.AsInteger());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsFloating());
  ASSERT_FALSE(property.IsBoolean());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsFile());
  ASSERT_FALSE(property.IsObject());
}

TEST(Property, FloatProperty)
{
  const Property property{12.3f};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsFloating());
  ASSERT_TRUE(property.TryAsFloating());
  ASSERT_EQ(12.3f, property.AsFloating());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsInteger());
  ASSERT_FALSE(property.IsBoolean());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsFile());
  ASSERT_FALSE(property.IsObject());
}

TEST(Property, StringProperty)
{
  const Property property{"foo"s};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsString());
  ASSERT_TRUE(property.TryAsString());
  ASSERT_EQ("foo", property.AsString());

  ASSERT_FALSE(property.IsInteger());
  ASSERT_FALSE(property.IsFloating());
  ASSERT_FALSE(property.IsBoolean());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsFile());
  ASSERT_FALSE(property.IsObject());
}

TEST(Property, BoolProperty)
{
  const Property property{false};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsBoolean());
  ASSERT_TRUE(property.TryAsBoolean());
  ASSERT_FALSE(property.AsBoolean());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsInteger());
  ASSERT_FALSE(property.IsFloating());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsFile());
  ASSERT_FALSE(property.IsObject());
}

TEST(Property, FileProperty)
{
  const std::filesystem::path file{"resources/foo.txt"};
  const Property property{file};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsFile());
  ASSERT_TRUE(property.TryAsFile());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsInteger());
  ASSERT_FALSE(property.IsFloating());
  ASSERT_FALSE(property.IsBoolean());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsObject());
}

TEST(Property, ObjectProperty)
{
  const Property property{object_ref{7}};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsObject());
  ASSERT_TRUE(property.TryAsObject());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsInteger());
  ASSERT_FALSE(property.IsFloating());
  ASSERT_FALSE(property.IsBoolean());
  ASSERT_FALSE(property.IsColor());
  ASSERT_FALSE(property.IsFile());
}

TEST(Property, ColorProperty)
{
  const auto color = cen::colors::red;
  const Property property{color};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsColor());
  ASSERT_TRUE(property.TryAsColor());

  ASSERT_FALSE(property.IsString());
  ASSERT_FALSE(property.IsInteger());
  ASSERT_FALSE(property.IsFloating());
  ASSERT_FALSE(property.IsBoolean());
  ASSERT_FALSE(property.IsFile());
  ASSERT_FALSE(property.IsObject());
}

TEST(Property, Reset)
{
  Property property;
  ASSERT_FALSE(property.HasValue());

  property.SetValue(123);
  ASSERT_TRUE(property.HasValue());

  property.Reset();
  ASSERT_FALSE(property.HasValue());

  ASSERT_NO_THROW(property.Reset());
  ASSERT_NO_THROW(property.Reset());
}

TEST(Property, SetValue)
{
  Property property;

  property.SetValue(10);
  ASSERT_TRUE(property.IsInteger());
  ASSERT_EQ(10, property.AsInteger());

  property.SetValue(93.2f);
  ASSERT_TRUE(property.IsFloating());
  ASSERT_EQ(93.2f, property.AsFloating());

  property.SetValue(true);
  ASSERT_TRUE(property.IsBoolean());
  ASSERT_TRUE(property.AsBoolean());

  property.SetValue("foo"s);
  ASSERT_TRUE(property.IsString());
  ASSERT_EQ("foo", property.AsString());
}

TEST(Property, HasValue)
{
  Property property;
  ASSERT_FALSE(property.HasValue());

  property.SetValue(924);
  ASSERT_TRUE(property.HasValue());
}

TEST(Property, As)
{
  const Property property{123};

  ASSERT_NO_THROW(property.AsInteger());
  ASSERT_ANY_THROW(property.AsFloating());
  ASSERT_ANY_THROW(property.AsString());
  ASSERT_ANY_THROW(property.AsBoolean());
  ASSERT_ANY_THROW(property.AsObject());
  ASSERT_ANY_THROW(property.AsFile());
  ASSERT_ANY_THROW(property.AsColor());
}

TEST(Property, TryAs)
{
  Property property;

  ASSERT_FALSE(property.TryAsInteger());
  ASSERT_FALSE(property.TryAsFloating());
  ASSERT_FALSE(property.TryAsString());
  ASSERT_FALSE(property.TryAsBoolean());

  property.SetValue(123);
  ASSERT_TRUE(property.TryAsInteger());
  ASSERT_FALSE(property.TryAsFloating());
  ASSERT_FALSE(property.TryAsString());
  ASSERT_FALSE(property.TryAsBoolean());
}

TEST(Property, GetType)
{
  Property property;
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
