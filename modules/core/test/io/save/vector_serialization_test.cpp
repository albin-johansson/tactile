// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/save/vector_serialization.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(VectorSerialization, SerializeInt2)
{
  EXPECT_EQ(serialize(Int2 {-42, 942}), "-42;942");
}

TEST(VectorSerialization, SerializeInt3)
{
  EXPECT_EQ(serialize(Int3 {539, 84'239, -12}), "539;84239;-12");
}

TEST(VectorSerialization, SerializeInt4)
{
  EXPECT_EQ(serialize(Int4 {512, 994, 5, 75}), "512;994;5;75");
}

TEST(VectorSerialization, SerializeFloat2)
{
  EXPECT_EQ(serialize(Float2 {1.5f, -83.8f}), "1.5;-83.8");
}

TEST(VectorSerialization, SerializeFloat3)
{
  EXPECT_EQ(serialize(Float3 {94.4f, 100.0f, -5.0f}), "94.4;100;-5");
}

TEST(VectorSerialization, SerializeFloat4)
{
  EXPECT_EQ(serialize(Float4 {1.0f, 2.0f, 3.0f, 4.0f}), "1;2;3;4");
}
