// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/context/attribute.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(Attribute, DefaultConstructor)
{
  const Attribute attribute;
  EXPECT_EQ(attribute.get_type(), AttributeType::Str);
  EXPECT_TRUE(attribute.has_default_value());
}

TEST(Attribute, ExplicitTypeConstructor)
{
  EXPECT_EQ(Attribute {AttributeType::Str}, String {});
  EXPECT_EQ(Attribute {AttributeType::Int}, int32 {});
  EXPECT_EQ(Attribute {AttributeType::Int2}, Int2 {});
  EXPECT_EQ(Attribute {AttributeType::Int3}, Int3 {});
  EXPECT_EQ(Attribute {AttributeType::Int4}, Int4 {});
  EXPECT_EQ(Attribute {AttributeType::Float}, float32 {});
  EXPECT_EQ(Attribute {AttributeType::Float2}, Float2 {});
  EXPECT_EQ(Attribute {AttributeType::Float3}, Float3 {});
  EXPECT_EQ(Attribute {AttributeType::Float4}, Float4 {});
  EXPECT_EQ(Attribute {AttributeType::Bool}, bool {});
  EXPECT_EQ(Attribute {AttributeType::Color}, UColor {});
  EXPECT_EQ(Attribute {AttributeType::Path}, fs::Path {});
  EXPECT_EQ(Attribute {AttributeType::Object}, ObjectRef {});
}

TEST(Attribute, ImplicitValueConstructor)
{
  {
    const Attribute str1 = "foo";
    const Attribute str2 = String {"bar"};

    EXPECT_EQ(str1.get_type(), AttributeType::Str);
    EXPECT_EQ(str2.get_type(), AttributeType::Str);

    EXPECT_EQ(str1.as_string(), "foo");
    EXPECT_EQ(str2.as_string(), "bar");
  }

  {
    const Attribute integer = 42;
    EXPECT_EQ(integer.get_type(), AttributeType::Int);
    EXPECT_EQ(integer.as_int(), 42);
  }

  {
    const Attribute real = 1.5f;
    EXPECT_EQ(real.get_type(), AttributeType::Float);
    EXPECT_EQ(real.as_float(), 1.5f);
  }

  {
    const Attribute yes = true;
    const Attribute no = false;

    EXPECT_EQ(yes.get_type(), AttributeType::Bool);
    EXPECT_EQ(no.get_type(), AttributeType::Bool);

    EXPECT_TRUE(yes.as_bool());
    EXPECT_FALSE(no.as_bool());
  }

  {
    const Attribute color = UColor {1, 2, 3, 4};
    EXPECT_EQ(color.get_type(), AttributeType::Color);
    EXPECT_EQ(color.as_color(), (UColor {1, 2, 3, 4}));
  }

  {
    const Attribute path = std::filesystem::path {"foo/bar"};
    EXPECT_EQ(path.get_type(), AttributeType::Path);
    EXPECT_EQ(path.as_path(), "foo/bar");
  }

  {
    const Attribute object = ObjectRef {100};
    EXPECT_EQ(object.get_type(), AttributeType::Object);
    EXPECT_EQ(object.as_object(), ObjectRef {100});
  }
}

TEST(Attribute, ImplicitCStringConstructor)
{
  const Attribute attribute = "foobar";
  EXPECT_FALSE(attribute.has_default_value());
  EXPECT_EQ(attribute.get_type(), AttributeType::Str);
  EXPECT_EQ(attribute.as_string(), "foobar");
  EXPECT_EQ(attribute, "foobar");
}

TEST(Attribute, Reset)
{
  Attribute attribute;

  attribute.reset(AttributeType::Str);
  EXPECT_TRUE(attribute.has_default_value());
  EXPECT_EQ(attribute, String {});

  attribute.reset(AttributeType::Int);
  EXPECT_TRUE(attribute.has_default_value());
  EXPECT_EQ(attribute, int32 {});

  attribute.reset(AttributeType::Int2);
  EXPECT_TRUE(attribute.has_default_value());
  EXPECT_EQ(attribute, Int2 {});

  attribute.reset(AttributeType::Int3);
  EXPECT_TRUE(attribute.has_default_value());
  EXPECT_EQ(attribute, Int3 {});

  attribute.reset(AttributeType::Int4);
  EXPECT_TRUE(attribute.has_default_value());
  EXPECT_EQ(attribute, Int4 {});

  attribute.reset(AttributeType::Float);
  EXPECT_TRUE(attribute.has_default_value());
  EXPECT_EQ(attribute, float32 {});

  attribute.reset(AttributeType::Float2);
  EXPECT_TRUE(attribute.has_default_value());
  EXPECT_EQ(attribute, Float2 {});

  attribute.reset(AttributeType::Float3);
  EXPECT_TRUE(attribute.has_default_value());
  EXPECT_EQ(attribute, Float3 {});

  attribute.reset(AttributeType::Float4);
  EXPECT_TRUE(attribute.has_default_value());
  EXPECT_EQ(attribute, Float4 {});

  attribute.reset(AttributeType::Bool);
  EXPECT_TRUE(attribute.has_default_value());
  EXPECT_EQ(attribute, bool {});

  attribute.reset(AttributeType::Color);
  EXPECT_TRUE(attribute.has_default_value());
  EXPECT_EQ(attribute, UColor {});

  attribute.reset(AttributeType::Path);
  EXPECT_TRUE(attribute.has_default_value());
  EXPECT_EQ(attribute, fs::Path {});

  attribute.reset(AttributeType::Object);
  EXPECT_TRUE(attribute.has_default_value());
  EXPECT_EQ(attribute, ObjectRef {});
}

TEST(Attribute, Set)
{
  Attribute attribute;
  EXPECT_TRUE(attribute.has_default_value());

  attribute.set("foo");
  EXPECT_EQ(attribute, "foo");
  EXPECT_EQ(attribute.get_type(), AttributeType::Str);
  EXPECT_FALSE(attribute.has_default_value());

  attribute.set(0);
  EXPECT_EQ(attribute, 0);
  EXPECT_EQ(attribute.get_type(), AttributeType::Int);
  EXPECT_TRUE(attribute.has_default_value());

  attribute.set(Float3 {1, 2, 3});
  EXPECT_EQ(attribute, (Float3 {1, 2, 3}));
  EXPECT_EQ(attribute.get_type(), AttributeType::Float3);
  EXPECT_FALSE(attribute.has_default_value());
}

TEST(Attribute, AsString)
{
  EXPECT_EQ(Attribute {AttributeType::Str}.as_string(), "");
  EXPECT_THROW((void) Attribute {AttributeType::Int}.as_string(), Error);
}

TEST(Attribute, AsInt)
{
  EXPECT_EQ(Attribute {AttributeType::Int}.as_int(), 0);
  EXPECT_THROW((void) Attribute {AttributeType::Color}.as_int(), Error);
}

TEST(Attribute, AsInt2)
{
  EXPECT_EQ(Attribute {AttributeType::Int2}.as_int2(), Int2 {});
  EXPECT_THROW((void) Attribute {AttributeType::Path}.as_int2(), Error);
}

TEST(Attribute, AsInt3)
{
  EXPECT_EQ(Attribute {AttributeType::Int3}.as_int3(), Int3 {});
  EXPECT_THROW((void) Attribute {AttributeType::Int}.as_int3(), Error);
}

TEST(Attribute, AsInt4)
{
  EXPECT_EQ(Attribute {AttributeType::Int4}.as_int4(), Int4 {});
  EXPECT_THROW((void) Attribute {AttributeType::Float4}.as_int4(), Error);
}

TEST(Attribute, AsFloat)
{
  EXPECT_EQ(Attribute {AttributeType::Float}.as_float(), 0.0f);
  EXPECT_THROW((void) Attribute {AttributeType::Str}.as_float(), Error);
}

TEST(Attribute, AsFloat2)
{
  EXPECT_EQ(Attribute {AttributeType::Float2}.as_float2(), Float2 {});
  EXPECT_THROW((void) Attribute {AttributeType::Bool}.as_float2(), Error);
}

TEST(Attribute, AsFloat3)
{
  EXPECT_EQ(Attribute {AttributeType::Float3}.as_float3(), Float3 {});
  EXPECT_THROW((void) Attribute {AttributeType::Object}.as_float3(), Error);
}

TEST(Attribute, AsFloat4)
{
  EXPECT_EQ(Attribute {AttributeType::Float4}.as_float4(), Float4 {});
  EXPECT_THROW((void) Attribute {AttributeType::Float3}.as_float4(), Error);
}

TEST(Attribute, AsBool)
{
  EXPECT_EQ(Attribute {AttributeType::Bool}.as_bool(), false);
  EXPECT_THROW((void) Attribute {AttributeType::Int}.as_bool(), Error);
}

TEST(Attribute, AsColor)
{
  EXPECT_EQ(Attribute {AttributeType::Color}.as_color(), UColor {});
  EXPECT_THROW((void) Attribute {AttributeType::Str}.as_color(), Error);
}

TEST(Attribute, AsPath)
{
  EXPECT_EQ(Attribute {AttributeType::Path}.as_path(), fs::Path {});
  EXPECT_THROW((void) Attribute {AttributeType::Str}.as_path(), Error);
}

TEST(Attribute, AsObject)
{
  EXPECT_EQ(Attribute {AttributeType::Object}.as_object(), ObjectRef {});
  EXPECT_THROW((void) Attribute {AttributeType::Int}.as_object(), Error);
}

TEST(Attribute, IsVector)
{
  EXPECT_TRUE(Attribute {AttributeType::Int2}.is_vector());
  EXPECT_TRUE(Attribute {AttributeType::Int3}.is_vector());
  EXPECT_TRUE(Attribute {AttributeType::Int4}.is_vector());
  EXPECT_TRUE(Attribute {AttributeType::Float2}.is_vector());
  EXPECT_TRUE(Attribute {AttributeType::Float3}.is_vector());
  EXPECT_TRUE(Attribute {AttributeType::Float4}.is_vector());

  EXPECT_FALSE(Attribute {AttributeType::Str}.is_vector());
  EXPECT_FALSE(Attribute {AttributeType::Int}.is_vector());
  EXPECT_FALSE(Attribute {AttributeType::Float}.is_vector());
  EXPECT_FALSE(Attribute {AttributeType::Bool}.is_vector());
  EXPECT_FALSE(Attribute {AttributeType::Color}.is_vector());
  EXPECT_FALSE(Attribute {AttributeType::Path}.is_vector());
  EXPECT_FALSE(Attribute {AttributeType::Object}.is_vector());
}
