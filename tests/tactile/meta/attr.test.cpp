// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include <gmock/gmock.h>
#include <gtest/gtest.h>

import tactile.meta;

namespace tactile::tests {
namespace {

using testing::Const;

class AttrTest : public testing::Test
{};

TEST_F(AttrTest, Defaults)
{
  const Attr attr {};
  EXPECT_EQ(attr.kind(), AttrKind::kInt);
  EXPECT_TRUE(attr.has_default_value());
}

TEST_F(AttrTest, Int)
{
  Attr attr {AttrKind::kInt};

  EXPECT_EQ(attr.kind(), AttrKind::kInt);

  ASSERT_NE(attr.as_int(), nullptr);
  ASSERT_EQ(attr.as_int2(), nullptr);
  ASSERT_EQ(attr.as_int3(), nullptr);
  ASSERT_EQ(attr.as_int4(), nullptr);
  ASSERT_EQ(attr.as_float(), nullptr);
  ASSERT_EQ(attr.as_float2(), nullptr);
  ASSERT_EQ(attr.as_float3(), nullptr);
  ASSERT_EQ(attr.as_float4(), nullptr);
  ASSERT_EQ(attr.as_bool(), nullptr);
  ASSERT_EQ(attr.as_string(), nullptr);
  ASSERT_EQ(attr.as_path(), nullptr);
  ASSERT_EQ(attr.as_color(), nullptr);

  ASSERT_NE(Const(attr).as_int(), nullptr);
  ASSERT_EQ(Const(attr).as_int2(), nullptr);
  ASSERT_EQ(Const(attr).as_int3(), nullptr);
  ASSERT_EQ(Const(attr).as_int4(), nullptr);
  ASSERT_EQ(Const(attr).as_float(), nullptr);
  ASSERT_EQ(Const(attr).as_float2(), nullptr);
  ASSERT_EQ(Const(attr).as_float3(), nullptr);
  ASSERT_EQ(Const(attr).as_float4(), nullptr);
  ASSERT_EQ(Const(attr).as_bool(), nullptr);
  ASSERT_EQ(Const(attr).as_string(), nullptr);
  ASSERT_EQ(Const(attr).as_path(), nullptr);
  ASSERT_EQ(Const(attr).as_color(), nullptr);

  EXPECT_TRUE(attr.has_default_value());
  EXPECT_FALSE(attr.is_vector());
}

TEST_F(AttrTest, Int2)
{
  Attr attr {AttrKind::kInt2};

  EXPECT_EQ(attr.kind(), AttrKind::kInt2);

  ASSERT_EQ(attr.as_int(), nullptr);
  ASSERT_NE(attr.as_int2(), nullptr);
  ASSERT_EQ(attr.as_int3(), nullptr);
  ASSERT_EQ(attr.as_int4(), nullptr);
  ASSERT_EQ(attr.as_float(), nullptr);
  ASSERT_EQ(attr.as_float2(), nullptr);
  ASSERT_EQ(attr.as_float3(), nullptr);
  ASSERT_EQ(attr.as_float4(), nullptr);
  ASSERT_EQ(attr.as_bool(), nullptr);
  ASSERT_EQ(attr.as_string(), nullptr);
  ASSERT_EQ(attr.as_path(), nullptr);
  ASSERT_EQ(attr.as_color(), nullptr);

  ASSERT_EQ(Const(attr).as_int(), nullptr);
  ASSERT_NE(Const(attr).as_int2(), nullptr);
  ASSERT_EQ(Const(attr).as_int3(), nullptr);
  ASSERT_EQ(Const(attr).as_int4(), nullptr);
  ASSERT_EQ(Const(attr).as_float(), nullptr);
  ASSERT_EQ(Const(attr).as_float2(), nullptr);
  ASSERT_EQ(Const(attr).as_float3(), nullptr);
  ASSERT_EQ(Const(attr).as_float4(), nullptr);
  ASSERT_EQ(Const(attr).as_bool(), nullptr);
  ASSERT_EQ(Const(attr).as_string(), nullptr);
  ASSERT_EQ(Const(attr).as_path(), nullptr);
  ASSERT_EQ(Const(attr).as_color(), nullptr);

  EXPECT_TRUE(attr.has_default_value());
  EXPECT_TRUE(attr.is_vector());
}

TEST_F(AttrTest, Int3)
{
  Attr attr {AttrKind::kInt3};

  EXPECT_EQ(attr.kind(), AttrKind::kInt3);

  ASSERT_EQ(attr.as_int(), nullptr);
  ASSERT_EQ(attr.as_int2(), nullptr);
  ASSERT_NE(attr.as_int3(), nullptr);
  ASSERT_EQ(attr.as_int4(), nullptr);
  ASSERT_EQ(attr.as_float(), nullptr);
  ASSERT_EQ(attr.as_float2(), nullptr);
  ASSERT_EQ(attr.as_float3(), nullptr);
  ASSERT_EQ(attr.as_float4(), nullptr);
  ASSERT_EQ(attr.as_bool(), nullptr);
  ASSERT_EQ(attr.as_string(), nullptr);
  ASSERT_EQ(attr.as_path(), nullptr);
  ASSERT_EQ(attr.as_color(), nullptr);

  ASSERT_EQ(Const(attr).as_int(), nullptr);
  ASSERT_EQ(Const(attr).as_int2(), nullptr);
  ASSERT_NE(Const(attr).as_int3(), nullptr);
  ASSERT_EQ(Const(attr).as_int4(), nullptr);
  ASSERT_EQ(Const(attr).as_float(), nullptr);
  ASSERT_EQ(Const(attr).as_float2(), nullptr);
  ASSERT_EQ(Const(attr).as_float3(), nullptr);
  ASSERT_EQ(Const(attr).as_float4(), nullptr);
  ASSERT_EQ(Const(attr).as_bool(), nullptr);
  ASSERT_EQ(Const(attr).as_string(), nullptr);
  ASSERT_EQ(Const(attr).as_path(), nullptr);
  ASSERT_EQ(Const(attr).as_color(), nullptr);

  EXPECT_TRUE(attr.has_default_value());
  EXPECT_TRUE(attr.is_vector());
}

TEST_F(AttrTest, Int4)
{
  Attr attr {AttrKind::kInt4};

  EXPECT_EQ(attr.kind(), AttrKind::kInt4);

  ASSERT_EQ(attr.as_int(), nullptr);
  ASSERT_EQ(attr.as_int2(), nullptr);
  ASSERT_EQ(attr.as_int3(), nullptr);
  ASSERT_NE(attr.as_int4(), nullptr);
  ASSERT_EQ(attr.as_float(), nullptr);
  ASSERT_EQ(attr.as_float2(), nullptr);
  ASSERT_EQ(attr.as_float3(), nullptr);
  ASSERT_EQ(attr.as_float4(), nullptr);
  ASSERT_EQ(attr.as_bool(), nullptr);
  ASSERT_EQ(attr.as_string(), nullptr);
  ASSERT_EQ(attr.as_path(), nullptr);
  ASSERT_EQ(attr.as_color(), nullptr);

  ASSERT_EQ(Const(attr).as_int(), nullptr);
  ASSERT_EQ(Const(attr).as_int2(), nullptr);
  ASSERT_EQ(Const(attr).as_int3(), nullptr);
  ASSERT_NE(Const(attr).as_int4(), nullptr);
  ASSERT_EQ(Const(attr).as_float(), nullptr);
  ASSERT_EQ(Const(attr).as_float2(), nullptr);
  ASSERT_EQ(Const(attr).as_float3(), nullptr);
  ASSERT_EQ(Const(attr).as_float4(), nullptr);
  ASSERT_EQ(Const(attr).as_bool(), nullptr);
  ASSERT_EQ(Const(attr).as_string(), nullptr);
  ASSERT_EQ(Const(attr).as_path(), nullptr);
  ASSERT_EQ(Const(attr).as_color(), nullptr);

  EXPECT_TRUE(attr.has_default_value());
  EXPECT_TRUE(attr.is_vector());
}

TEST_F(AttrTest, Float2)
{
  Attr attr {AttrKind::kFloat2};

  EXPECT_EQ(attr.kind(), AttrKind::kFloat2);

  ASSERT_EQ(attr.as_int(), nullptr);
  ASSERT_EQ(attr.as_int2(), nullptr);
  ASSERT_EQ(attr.as_int3(), nullptr);
  ASSERT_EQ(attr.as_int4(), nullptr);
  ASSERT_EQ(attr.as_float(), nullptr);
  ASSERT_NE(attr.as_float2(), nullptr);
  ASSERT_EQ(attr.as_float3(), nullptr);
  ASSERT_EQ(attr.as_float4(), nullptr);
  ASSERT_EQ(attr.as_bool(), nullptr);
  ASSERT_EQ(attr.as_string(), nullptr);
  ASSERT_EQ(attr.as_path(), nullptr);
  ASSERT_EQ(attr.as_color(), nullptr);

  ASSERT_EQ(Const(attr).as_int(), nullptr);
  ASSERT_EQ(Const(attr).as_int2(), nullptr);
  ASSERT_EQ(Const(attr).as_int3(), nullptr);
  ASSERT_EQ(Const(attr).as_int4(), nullptr);
  ASSERT_EQ(Const(attr).as_float(), nullptr);
  ASSERT_NE(Const(attr).as_float2(), nullptr);
  ASSERT_EQ(Const(attr).as_float3(), nullptr);
  ASSERT_EQ(Const(attr).as_float4(), nullptr);
  ASSERT_EQ(Const(attr).as_bool(), nullptr);
  ASSERT_EQ(Const(attr).as_string(), nullptr);
  ASSERT_EQ(Const(attr).as_path(), nullptr);
  ASSERT_EQ(Const(attr).as_color(), nullptr);

  EXPECT_TRUE(attr.has_default_value());
  EXPECT_TRUE(attr.is_vector());
}

TEST_F(AttrTest, Float3)
{
  Attr attr {AttrKind::kFloat3};

  EXPECT_EQ(attr.kind(), AttrKind::kFloat3);

  ASSERT_EQ(attr.as_int(), nullptr);
  ASSERT_EQ(attr.as_int2(), nullptr);
  ASSERT_EQ(attr.as_int3(), nullptr);
  ASSERT_EQ(attr.as_int4(), nullptr);
  ASSERT_EQ(attr.as_float(), nullptr);
  ASSERT_EQ(attr.as_float2(), nullptr);
  ASSERT_NE(attr.as_float3(), nullptr);
  ASSERT_EQ(attr.as_float4(), nullptr);
  ASSERT_EQ(attr.as_bool(), nullptr);
  ASSERT_EQ(attr.as_string(), nullptr);
  ASSERT_EQ(attr.as_path(), nullptr);
  ASSERT_EQ(attr.as_color(), nullptr);

  ASSERT_EQ(Const(attr).as_int(), nullptr);
  ASSERT_EQ(Const(attr).as_int2(), nullptr);
  ASSERT_EQ(Const(attr).as_int3(), nullptr);
  ASSERT_EQ(Const(attr).as_int4(), nullptr);
  ASSERT_EQ(Const(attr).as_float(), nullptr);
  ASSERT_EQ(Const(attr).as_float2(), nullptr);
  ASSERT_NE(Const(attr).as_float3(), nullptr);
  ASSERT_EQ(Const(attr).as_float4(), nullptr);
  ASSERT_EQ(Const(attr).as_bool(), nullptr);
  ASSERT_EQ(Const(attr).as_string(), nullptr);
  ASSERT_EQ(Const(attr).as_path(), nullptr);
  ASSERT_EQ(Const(attr).as_color(), nullptr);

  EXPECT_TRUE(attr.has_default_value());
  EXPECT_TRUE(attr.is_vector());
}

TEST_F(AttrTest, Float4)
{
  Attr attr {AttrKind::kFloat4};

  EXPECT_EQ(attr.kind(), AttrKind::kFloat4);

  ASSERT_EQ(attr.as_int(), nullptr);
  ASSERT_EQ(attr.as_int2(), nullptr);
  ASSERT_EQ(attr.as_int3(), nullptr);
  ASSERT_EQ(attr.as_int4(), nullptr);
  ASSERT_EQ(attr.as_float(), nullptr);
  ASSERT_EQ(attr.as_float2(), nullptr);
  ASSERT_EQ(attr.as_float3(), nullptr);
  ASSERT_NE(attr.as_float4(), nullptr);
  ASSERT_EQ(attr.as_bool(), nullptr);
  ASSERT_EQ(attr.as_string(), nullptr);
  ASSERT_EQ(attr.as_path(), nullptr);
  ASSERT_EQ(attr.as_color(), nullptr);

  ASSERT_EQ(Const(attr).as_int(), nullptr);
  ASSERT_EQ(Const(attr).as_int2(), nullptr);
  ASSERT_EQ(Const(attr).as_int3(), nullptr);
  ASSERT_EQ(Const(attr).as_int4(), nullptr);
  ASSERT_EQ(Const(attr).as_float(), nullptr);
  ASSERT_EQ(Const(attr).as_float2(), nullptr);
  ASSERT_EQ(Const(attr).as_float3(), nullptr);
  ASSERT_NE(Const(attr).as_float4(), nullptr);
  ASSERT_EQ(Const(attr).as_bool(), nullptr);
  ASSERT_EQ(Const(attr).as_string(), nullptr);
  ASSERT_EQ(Const(attr).as_path(), nullptr);
  ASSERT_EQ(Const(attr).as_color(), nullptr);

  EXPECT_TRUE(attr.has_default_value());
  EXPECT_TRUE(attr.is_vector());
}

TEST_F(AttrTest, Bool)
{
  Attr attr {AttrKind::kBool};

  EXPECT_EQ(attr.kind(), AttrKind::kBool);

  ASSERT_EQ(attr.as_int(), nullptr);
  ASSERT_EQ(attr.as_int2(), nullptr);
  ASSERT_EQ(attr.as_int3(), nullptr);
  ASSERT_EQ(attr.as_int4(), nullptr);
  ASSERT_EQ(attr.as_float(), nullptr);
  ASSERT_EQ(attr.as_float2(), nullptr);
  ASSERT_EQ(attr.as_float3(), nullptr);
  ASSERT_EQ(attr.as_float4(), nullptr);
  ASSERT_NE(attr.as_bool(), nullptr);
  ASSERT_EQ(attr.as_string(), nullptr);
  ASSERT_EQ(attr.as_path(), nullptr);
  ASSERT_EQ(attr.as_color(), nullptr);

  ASSERT_EQ(Const(attr).as_int(), nullptr);
  ASSERT_EQ(Const(attr).as_int2(), nullptr);
  ASSERT_EQ(Const(attr).as_int3(), nullptr);
  ASSERT_EQ(Const(attr).as_int4(), nullptr);
  ASSERT_EQ(Const(attr).as_float(), nullptr);
  ASSERT_EQ(Const(attr).as_float2(), nullptr);
  ASSERT_EQ(Const(attr).as_float3(), nullptr);
  ASSERT_EQ(Const(attr).as_float4(), nullptr);
  ASSERT_NE(Const(attr).as_bool(), nullptr);
  ASSERT_EQ(Const(attr).as_string(), nullptr);
  ASSERT_EQ(Const(attr).as_path(), nullptr);
  ASSERT_EQ(Const(attr).as_color(), nullptr);

  EXPECT_TRUE(attr.has_default_value());
  EXPECT_FALSE(attr.is_vector());
}

TEST_F(AttrTest, String)
{
  Attr attr {AttrKind::kString};

  EXPECT_EQ(attr.kind(), AttrKind::kString);

  ASSERT_EQ(attr.as_int(), nullptr);
  ASSERT_EQ(attr.as_int2(), nullptr);
  ASSERT_EQ(attr.as_int3(), nullptr);
  ASSERT_EQ(attr.as_int4(), nullptr);
  ASSERT_EQ(attr.as_float(), nullptr);
  ASSERT_EQ(attr.as_float2(), nullptr);
  ASSERT_EQ(attr.as_float3(), nullptr);
  ASSERT_EQ(attr.as_float4(), nullptr);
  ASSERT_EQ(attr.as_bool(), nullptr);
  ASSERT_NE(attr.as_string(), nullptr);
  ASSERT_EQ(attr.as_path(), nullptr);
  ASSERT_EQ(attr.as_color(), nullptr);

  ASSERT_EQ(Const(attr).as_int(), nullptr);
  ASSERT_EQ(Const(attr).as_int2(), nullptr);
  ASSERT_EQ(Const(attr).as_int3(), nullptr);
  ASSERT_EQ(Const(attr).as_int4(), nullptr);
  ASSERT_EQ(Const(attr).as_float(), nullptr);
  ASSERT_EQ(Const(attr).as_float2(), nullptr);
  ASSERT_EQ(Const(attr).as_float3(), nullptr);
  ASSERT_EQ(Const(attr).as_float4(), nullptr);
  ASSERT_EQ(Const(attr).as_bool(), nullptr);
  ASSERT_NE(Const(attr).as_string(), nullptr);
  ASSERT_EQ(Const(attr).as_path(), nullptr);
  ASSERT_EQ(Const(attr).as_color(), nullptr);

  EXPECT_TRUE(attr.has_default_value());
  EXPECT_FALSE(attr.is_vector());
}

TEST_F(AttrTest, Path)
{
  Attr attr {AttrKind::kPath};

  EXPECT_EQ(attr.kind(), AttrKind::kPath);

  ASSERT_EQ(attr.as_int(), nullptr);
  ASSERT_EQ(attr.as_int2(), nullptr);
  ASSERT_EQ(attr.as_int3(), nullptr);
  ASSERT_EQ(attr.as_int4(), nullptr);
  ASSERT_EQ(attr.as_float(), nullptr);
  ASSERT_EQ(attr.as_float2(), nullptr);
  ASSERT_EQ(attr.as_float3(), nullptr);
  ASSERT_EQ(attr.as_float4(), nullptr);
  ASSERT_EQ(attr.as_bool(), nullptr);
  ASSERT_EQ(attr.as_string(), nullptr);
  ASSERT_NE(attr.as_path(), nullptr);
  ASSERT_EQ(attr.as_color(), nullptr);

  ASSERT_EQ(Const(attr).as_int(), nullptr);
  ASSERT_EQ(Const(attr).as_int2(), nullptr);
  ASSERT_EQ(Const(attr).as_int3(), nullptr);
  ASSERT_EQ(Const(attr).as_int4(), nullptr);
  ASSERT_EQ(Const(attr).as_float(), nullptr);
  ASSERT_EQ(Const(attr).as_float2(), nullptr);
  ASSERT_EQ(Const(attr).as_float3(), nullptr);
  ASSERT_EQ(Const(attr).as_float4(), nullptr);
  ASSERT_EQ(Const(attr).as_bool(), nullptr);
  ASSERT_EQ(Const(attr).as_string(), nullptr);
  ASSERT_NE(Const(attr).as_path(), nullptr);
  ASSERT_EQ(Const(attr).as_color(), nullptr);

  EXPECT_TRUE(attr.has_default_value());
  EXPECT_FALSE(attr.is_vector());
}

TEST_F(AttrTest, Color)
{
  Attr attr {AttrKind::kColor};

  EXPECT_EQ(attr.kind(), AttrKind::kColor);

  ASSERT_EQ(attr.as_int(), nullptr);
  ASSERT_EQ(attr.as_int2(), nullptr);
  ASSERT_EQ(attr.as_int3(), nullptr);
  ASSERT_EQ(attr.as_int4(), nullptr);
  ASSERT_EQ(attr.as_float(), nullptr);
  ASSERT_EQ(attr.as_float2(), nullptr);
  ASSERT_EQ(attr.as_float3(), nullptr);
  ASSERT_EQ(attr.as_float4(), nullptr);
  ASSERT_EQ(attr.as_bool(), nullptr);
  ASSERT_EQ(attr.as_string(), nullptr);
  ASSERT_EQ(attr.as_path(), nullptr);
  ASSERT_NE(attr.as_color(), nullptr);

  ASSERT_EQ(Const(attr).as_int(), nullptr);
  ASSERT_EQ(Const(attr).as_int2(), nullptr);
  ASSERT_EQ(Const(attr).as_int3(), nullptr);
  ASSERT_EQ(Const(attr).as_int4(), nullptr);
  ASSERT_EQ(Const(attr).as_float(), nullptr);
  ASSERT_EQ(Const(attr).as_float2(), nullptr);
  ASSERT_EQ(Const(attr).as_float3(), nullptr);
  ASSERT_EQ(Const(attr).as_float4(), nullptr);
  ASSERT_EQ(Const(attr).as_bool(), nullptr);
  ASSERT_EQ(Const(attr).as_string(), nullptr);
  ASSERT_EQ(Const(attr).as_path(), nullptr);
  ASSERT_NE(Const(attr).as_color(), nullptr);

  EXPECT_TRUE(attr.has_default_value());
  EXPECT_FALSE(attr.is_vector());
}

TEST_F(AttrTest, Reset)
{
  Attr attr {};

  attr.emplace<Attr::int_type>(42);
  EXPECT_EQ(attr.kind(), AttrKind::kInt);
  EXPECT_FALSE(attr.has_default_value());

  attr.reset(AttrKind::kFloat);
  EXPECT_EQ(attr.kind(), AttrKind::kFloat);
  EXPECT_TRUE(attr.has_default_value());
}

TEST_F(AttrTest, Emplace)
{
  Attr attr {};
  ASSERT_EQ(attr.as_string(), nullptr);

  const auto& str = attr.emplace<Attr::string_type>("foo");
  EXPECT_EQ(str, "foo");
  EXPECT_EQ(attr.kind(), AttrKind::kString);
  EXPECT_NE(attr.as_string(), nullptr);

  EXPECT_FALSE(attr.has_default_value());
}

TEST_F(AttrTest, Eq)
{
  Attr a {AttrKind::kInt};
  Attr b {AttrKind::kInt};
  EXPECT_EQ(a, b);

  b.emplace<Attr::int_type>(1);
  EXPECT_NE(a, b);
}

}  // namespace
}  // namespace tactile::tests
