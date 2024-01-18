// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/save/vector_deserialization.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(VectorDeserialization, DeserializeInt2)
{
  EXPECT_FALSE(deserialize_int2("X;34"));
  EXPECT_FALSE(deserialize_int2("12;34 "));
  EXPECT_FALSE(deserialize_int2("12:34"));

  const auto vec = deserialize_int2("12;34");
  ASSERT_TRUE(vec.has_value());
  EXPECT_EQ(vec->x(), 12);
  EXPECT_EQ(vec->y(), 34);
}

TEST(VectorDeserialization, DeserializeInt3)
{
  EXPECT_FALSE(deserialize_int3(";2;3"));
  EXPECT_FALSE(deserialize_int3("1;2; "));
  EXPECT_FALSE(deserialize_int3("1:2:3"));

  const auto vec = deserialize_int3("1;2;3");
  ASSERT_TRUE(vec.has_value());
  EXPECT_EQ(vec->x(), 1);
  EXPECT_EQ(vec->y(), 2);
  EXPECT_EQ(vec->z(), 3);
}

TEST(VectorDeserialization, DeserializeInt4)
{
  EXPECT_FALSE(deserialize_int4("1;2;3;"));
  EXPECT_FALSE(deserialize_int4("1;2;3; "));
  EXPECT_FALSE(deserialize_int4("1:2:3:4"));

  const auto vec = deserialize_int4("1;-2;3;4");
  ASSERT_TRUE(vec.has_value());
  EXPECT_EQ(vec->x(), 1);
  EXPECT_EQ(vec->y(), -2);
  EXPECT_EQ(vec->z(), 3);
  EXPECT_EQ(vec->w(), 4);
}

TEST(VectorDeserialization, DeserializeFloat2)
{
  EXPECT_FALSE(deserialize_float2("1;2 3"));
  EXPECT_FALSE(deserialize_float2("1;2.O"));
  EXPECT_FALSE(deserialize_float2("1^2;3"));

  const auto vec = deserialize_float2("1.9;2.8");
  ASSERT_TRUE(vec.has_value());
  EXPECT_EQ(vec->x(), 1.9f);
  EXPECT_EQ(vec->y(), 2.8f);
}

TEST(VectorDeserialization, DeserializeFloat3)
{
  EXPECT_FALSE(deserialize_float3(" -1.0;294;12;"));
  EXPECT_FALSE(deserialize_float3("1. 0;2.0;3.0"));
  EXPECT_FALSE(deserialize_float3("1.9:-2.3:3"));

  const auto vec = deserialize_float3("1.9;-2.3;8.4");
  ASSERT_TRUE(vec.has_value());
  EXPECT_EQ(vec->x(), 1.9f);
  EXPECT_EQ(vec->y(), -2.3f);
  EXPECT_EQ(vec->z(), 8.4f);
}

TEST(VectorDeserialization, DeserializeFloat4)
{
  EXPECT_FALSE(deserialize_float4("1.0;2.h;3.0;"));
  EXPECT_FALSE(deserialize_float4("1.0;2.0 ;3.0;"));
  EXPECT_FALSE(deserialize_float4("1.0:2.0:3.0.4.0"));

  const auto vec = deserialize_float4("1.0;2.0;3.0;4.0");
  ASSERT_TRUE(vec.has_value());
  EXPECT_EQ(vec->x(), 1.0f);
  EXPECT_EQ(vec->y(), 2.0f);
  EXPECT_EQ(vec->z(), 3.0f);
  EXPECT_EQ(vec->w(), 4.0f);
}
