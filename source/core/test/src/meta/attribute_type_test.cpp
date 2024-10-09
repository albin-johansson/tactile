// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/meta/attribute_type.hpp"

#include <gtest/gtest.h>

namespace tactile::core {

/** \trace tactile::core::parse_attribute_type */
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
  EXPECT_EQ(parse_attribute_type("file"), AttributeType::kPath);
  EXPECT_EQ(parse_attribute_type("path"), AttributeType::kPath);
  EXPECT_EQ(parse_attribute_type("color"), AttributeType::kColor);
  EXPECT_EQ(parse_attribute_type("object"), AttributeType::kObject);

  EXPECT_FALSE(parse_attribute_type("foo").has_value());
  EXPECT_FALSE(parse_attribute_type("int ").has_value());
}

/** \trace tactile::core::serialize [AttributeType] */
TEST(AttributeType, Serialize)
{
  EXPECT_EQ(serialize(AttributeType::kStr), "string");
  EXPECT_EQ(serialize(AttributeType::kInt), "int");
  EXPECT_EQ(serialize(AttributeType::kInt2), "int2");
  EXPECT_EQ(serialize(AttributeType::kInt3), "int3");
  EXPECT_EQ(serialize(AttributeType::kInt4), "int4");
  EXPECT_EQ(serialize(AttributeType::kFloat), "float");
  EXPECT_EQ(serialize(AttributeType::kFloat2), "float2");
  EXPECT_EQ(serialize(AttributeType::kFloat3), "float3");
  EXPECT_EQ(serialize(AttributeType::kFloat4), "float4");
  EXPECT_EQ(serialize(AttributeType::kBool), "bool");
  EXPECT_EQ(serialize(AttributeType::kPath), "path");
  EXPECT_EQ(serialize(AttributeType::kColor), "color");
  EXPECT_EQ(serialize(AttributeType::kObject), "object");
}

}  // namespace tactile::core
