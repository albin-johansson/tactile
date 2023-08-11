/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tactile/core/contexts/property.hpp"

#include <filesystem>  // path
#include <string>      // string

#include <gtest/gtest.h>

#include "tactile/core/common/error.hpp"

using namespace tactile;

TEST(Property, DefaultConstructor)
{
  const Property property;
  EXPECT_EQ(property.get_type(), PropertyType::Str);
  EXPECT_EQ(property.as_string(), "");
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_FALSE(property.is_vector());
}

TEST(Property, ExplicitTypeConstructor)
{
  EXPECT_EQ(Property {PropertyType::Str}, std::string {});
  EXPECT_EQ(Property {PropertyType::Int}, int32 {});
  EXPECT_EQ(Property {PropertyType::Int2}, Int2 {});
  EXPECT_EQ(Property {PropertyType::Int3}, Int3 {});
  EXPECT_EQ(Property {PropertyType::Int4}, Int4 {});
  EXPECT_EQ(Property {PropertyType::Float}, float32 {});
  EXPECT_EQ(Property {PropertyType::Float2}, Float2 {});
  EXPECT_EQ(Property {PropertyType::Float3}, Float3 {});
  EXPECT_EQ(Property {PropertyType::Float4}, Float4 {});
  EXPECT_EQ(Property {PropertyType::Bool}, bool {});
  EXPECT_EQ(Property {PropertyType::Color}, UColor {});
  EXPECT_EQ(Property {PropertyType::Path}, std::filesystem::path {});
  EXPECT_EQ(Property {PropertyType::Object}, ObjectRef {});
}

TEST(Property, ImplicitCStringConstructor)
{
  const Property property = "foobar";
  EXPECT_FALSE(property.has_initial_value());
  EXPECT_EQ(property.get_type(), PropertyType::Str);
  EXPECT_EQ(property.as_string(), "foobar");
  EXPECT_EQ(property, "foobar");
}

TEST(Property, ImplicitValueConstructor)
{
  {
    const Property str1 = "foo";
    const Property str2 = std::string {"bar"};

    EXPECT_EQ(str1.get_type(), PropertyType::Str);
    EXPECT_EQ(str2.get_type(), PropertyType::Str);

    EXPECT_EQ(str1.as_string(), "foo");
    EXPECT_EQ(str2.as_string(), "bar");
  }

  {
    const Property integer = 42;
    EXPECT_EQ(integer.get_type(), PropertyType::Int);
    EXPECT_EQ(integer.as_int(), 42);
  }

  {
    const Property real = 1.5f;
    EXPECT_EQ(real.get_type(), PropertyType::Float);
    EXPECT_EQ(real.as_float(), 1.5f);
  }

  {
    const Property yes = true;
    const Property no = false;

    EXPECT_EQ(yes.get_type(), PropertyType::Bool);
    EXPECT_EQ(no.get_type(), PropertyType::Bool);

    EXPECT_TRUE(yes.as_bool());
    EXPECT_FALSE(no.as_bool());
  }

  {
    const Property color = UColor {1, 2, 3, 4};
    EXPECT_EQ(color.get_type(), PropertyType::Color);
    EXPECT_EQ(color.as_color(), (UColor {1, 2, 3, 4}));
  }

  {
    const Property path = std::filesystem::path {"foo/bar"};
    EXPECT_EQ(path.get_type(), PropertyType::Path);
    EXPECT_EQ(path.as_path(), "foo/bar");
  }

  {
    const Property object = ObjectRef {100};
    EXPECT_EQ(object.get_type(), PropertyType::Object);
    EXPECT_EQ(object.as_object(), ObjectRef {100});
  }
}

TEST(Property, Set)
{
  Property property;
  EXPECT_TRUE(property.has_initial_value());

  property.set("foo");
  EXPECT_EQ(property, "foo");
  EXPECT_EQ(property.get_type(), PropertyType::Str);
  EXPECT_FALSE(property.has_initial_value());

  property.set(0);
  EXPECT_EQ(property, 0);
  EXPECT_EQ(property.get_type(), PropertyType::Int);
  EXPECT_TRUE(property.has_initial_value());

  property.set(Float3 {1, 2, 3});
  EXPECT_EQ(property, (Float3 {1, 2, 3}));
  EXPECT_EQ(property.get_type(), PropertyType::Float3);
  EXPECT_FALSE(property.has_initial_value());
}

TEST(Property, Reset)
{
  Property property;

  property.reset(PropertyType::Str);
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_EQ(property, std::string {});

  property.reset(PropertyType::Int);
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_EQ(property, 0);

  property.reset(PropertyType::Int2);
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_EQ(property, Int2 {});

  property.reset(PropertyType::Int3);
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_EQ(property, Int3 {});

  property.reset(PropertyType::Int4);
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_EQ(property, Int4 {});

  property.reset(PropertyType::Float);
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_EQ(property, 0.0f);

  property.reset(PropertyType::Float2);
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_EQ(property, Float2 {});

  property.reset(PropertyType::Float3);
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_EQ(property, Float3 {});

  property.reset(PropertyType::Float4);
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_EQ(property, Float4 {});

  property.reset(PropertyType::Bool);
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_EQ(property, false);

  property.reset(PropertyType::Color);
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_EQ(property, UColor {});

  property.reset(PropertyType::Path);
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_EQ(property, std::filesystem::path {});

  property.reset(PropertyType::Object);
  EXPECT_TRUE(property.has_initial_value());
  EXPECT_EQ(property, ObjectRef {});
}

TEST(Property, AsString)
{
  EXPECT_EQ(Property {PropertyType::Str}.as_string(), "");
  EXPECT_THROW((void) Property {PropertyType::Int}.as_string(), Error);
}

TEST(Property, AsInt)
{
  EXPECT_EQ(Property {PropertyType::Int}.as_int(), 0);
  EXPECT_THROW((void) Property {PropertyType::Color}.as_int(), Error);
}

TEST(Property, AsInt2)
{
  EXPECT_EQ(Property {PropertyType::Int2}.as_int2(), Int2 {});
  EXPECT_THROW((void) Property {PropertyType::Path}.as_int2(), Error);
}

TEST(Property, AsInt3)
{
  EXPECT_EQ(Property {PropertyType::Int3}.as_int3(), Int3 {});
  EXPECT_THROW((void) Property {PropertyType::Int}.as_int3(), Error);
}

TEST(Property, AsInt4)
{
  EXPECT_EQ(Property {PropertyType::Int4}.as_int4(), Int4 {});
  EXPECT_THROW((void) Property {PropertyType::Float4}.as_int4(), Error);
}

TEST(Property, AsFloat)
{
  EXPECT_EQ(Property {PropertyType::Float}.as_float(), 0.0f);
  EXPECT_THROW((void) Property {PropertyType::Str}.as_float(), Error);
}

TEST(Property, AsFloat2)
{
  EXPECT_EQ(Property {PropertyType::Float2}.as_float2(), Float2 {});
  EXPECT_THROW((void) Property {PropertyType::Bool}.as_float2(), Error);
}

TEST(Property, AsFloat3)
{
  EXPECT_EQ(Property {PropertyType::Float3}.as_float3(), Float3 {});
  EXPECT_THROW((void) Property {PropertyType::Object}.as_float3(), Error);
}

TEST(Property, AsFloat4)
{
  EXPECT_EQ(Property {PropertyType::Float4}.as_float4(), Float4 {});
  EXPECT_THROW((void) Property {PropertyType::Float3}.as_float4(), Error);
}

TEST(Property, AsBool)
{
  EXPECT_EQ(Property {PropertyType::Bool}.as_bool(), false);
  EXPECT_THROW((void) Property {PropertyType::Int}.as_bool(), Error);
}

TEST(Property, AsColor)
{
  EXPECT_EQ(Property {PropertyType::Color}.as_color(), UColor {});
  EXPECT_THROW((void) Property {PropertyType::Str}.as_color(), Error);
}

TEST(Property, AsPath)
{
  EXPECT_EQ(Property {PropertyType::Path}.as_path(), std::filesystem::path {});
  EXPECT_THROW((void) Property {PropertyType::Str}.as_path(), Error);
}

TEST(Property, AsObject)
{
  EXPECT_EQ(Property {PropertyType::Object}.as_object(), ObjectRef {});
  EXPECT_THROW((void) Property {PropertyType::Int}.as_object(), Error);
}

TEST(Property, IsVector)
{
  EXPECT_TRUE(Property {PropertyType::Int2}.is_vector());
  EXPECT_TRUE(Property {PropertyType::Int3}.is_vector());
  EXPECT_TRUE(Property {PropertyType::Int4}.is_vector());
  EXPECT_TRUE(Property {PropertyType::Float2}.is_vector());
  EXPECT_TRUE(Property {PropertyType::Float3}.is_vector());
  EXPECT_TRUE(Property {PropertyType::Float4}.is_vector());

  EXPECT_FALSE(Property {PropertyType::Str}.is_vector());
  EXPECT_FALSE(Property {PropertyType::Int}.is_vector());
  EXPECT_FALSE(Property {PropertyType::Float}.is_vector());
  EXPECT_FALSE(Property {PropertyType::Bool}.is_vector());
  EXPECT_FALSE(Property {PropertyType::Color}.is_vector());
  EXPECT_FALSE(Property {PropertyType::Path}.is_vector());
  EXPECT_FALSE(Property {PropertyType::Object}.is_vector());
}
