// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/context/attribute_type.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(AttributeType, ParseAttributeType)
{
  EXPECT_EQ(parse_attribute_type("string"), AttributeType::kStr);
  EXPECT_EQ(parse_attribute_type("int"), AttributeType::kInt);
  EXPECT_EQ(parse_attribute_type("int2"), AttributeType::kInt2);
  EXPECT_EQ(parse_attribute_type("int3"), AttributeType::kInt3);
  EXPECT_EQ(parse_attribute_type("int4"), AttributeType::kInt4);
  EXPECT_EQ(parse_attribute_type("float"), AttributeType::kFloat);
  EXPECT_EQ(parse_attribute_type("float2"), AttributeType::kFloat2);
  EXPECT_EQ(parse_attribute_type("float3"), AttributeType::kFloat3);
  EXPECT_EQ(parse_attribute_type("float4"), AttributeType::kFloat4);
  EXPECT_EQ(parse_attribute_type("bool"), AttributeType::kBool);
  EXPECT_EQ(parse_attribute_type("path"), AttributeType::kPath);
  EXPECT_EQ(parse_attribute_type("file"), AttributeType::kPath);
  EXPECT_EQ(parse_attribute_type("color"), AttributeType::kColor);
  EXPECT_EQ(parse_attribute_type("object"), AttributeType::kObject);

  EXPECT_FALSE(parse_attribute_type("").has_value());
  EXPECT_FALSE(parse_attribute_type("String").has_value());
  EXPECT_FALSE(parse_attribute_type("int1").has_value());
}
