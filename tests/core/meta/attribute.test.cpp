// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/meta/attribute.hpp"

#include <stdexcept>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace tactile {
namespace {

using testing::Const;

class AttributeTest : public testing::Test
{};

TEST_F(AttributeTest, Defaults)
{
  const Attribute attribute {};

  EXPECT_EQ(attribute.get_kind(), AttributeKind::kInt);
  EXPECT_EQ(attribute.has_default_value(), true);

  ASSERT_NE(attribute.as_int(), nullptr);
  ASSERT_NE(Const(attribute).as_int(), nullptr);
  EXPECT_EQ(*attribute.as_int(), 0);
  EXPECT_EQ(*Const(attribute).as_int(), 0);

  EXPECT_EQ(attribute.as_float(), nullptr);
  EXPECT_EQ(Const(attribute).as_float(), nullptr);

  EXPECT_EQ(attribute.as_bool(), nullptr);
  EXPECT_EQ(Const(attribute).as_bool(), nullptr);

  EXPECT_EQ(attribute.as_string(), nullptr);
  EXPECT_EQ(Const(attribute).as_string(), nullptr);

  EXPECT_EQ(attribute.as_path(), nullptr);
  EXPECT_EQ(Const(attribute).as_path(), nullptr);

  EXPECT_EQ(attribute.as_color(), nullptr);
  EXPECT_EQ(Const(attribute).as_color(), nullptr);
}

TEST_F(AttributeTest, ResetToInt)
{
  Attribute attribute {};
  attribute.reset(AttributeKind::kInt);

  EXPECT_EQ(attribute.get_kind(), AttributeKind::kInt);
  EXPECT_EQ(attribute.has_default_value(), true);

  ASSERT_NE(attribute.as_int(), nullptr);
  EXPECT_EQ(*attribute.as_int(), 0);

  ASSERT_NE(Const(attribute).as_int(), nullptr);
  EXPECT_EQ(*Const(attribute).as_int(), 0);
}

TEST_F(AttributeTest, ResetToFloat)
{
  Attribute attribute {};
  attribute.reset(AttributeKind::kFloat);

  EXPECT_EQ(attribute.get_kind(), AttributeKind::kFloat);
  EXPECT_EQ(attribute.has_default_value(), true);

  ASSERT_NE(attribute.as_float(), nullptr);
  EXPECT_EQ(*attribute.as_float(), 0.0);

  ASSERT_NE(Const(attribute).as_float(), nullptr);
  EXPECT_EQ(*Const(attribute).as_float(), 0.0);
}

TEST_F(AttributeTest, ResetToBool)
{
  Attribute attribute {};
  attribute.reset(AttributeKind::kBool);

  EXPECT_EQ(attribute.get_kind(), AttributeKind::kBool);
  EXPECT_EQ(attribute.has_default_value(), true);

  ASSERT_NE(attribute.as_bool(), nullptr);
  EXPECT_EQ(*attribute.as_bool(), false);

  ASSERT_NE(Const(attribute).as_bool(), nullptr);
  EXPECT_EQ(*Const(attribute).as_bool(), false);
}

TEST_F(AttributeTest, ResetToString)
{
  Attribute attribute {};
  attribute.reset(AttributeKind::kString);

  EXPECT_EQ(attribute.get_kind(), AttributeKind::kString);
  EXPECT_EQ(attribute.has_default_value(), true);

  ASSERT_NE(attribute.as_string(), nullptr);
  EXPECT_EQ(*attribute.as_string(), "");

  ASSERT_NE(Const(attribute).as_string(), nullptr);
  EXPECT_EQ(*Const(attribute).as_string(), "");
}

TEST_F(AttributeTest, ResetToPath)
{
  Attribute attribute {};
  attribute.reset(AttributeKind::kPath);

  EXPECT_EQ(attribute.get_kind(), AttributeKind::kPath);
  EXPECT_EQ(attribute.has_default_value(), true);

  ASSERT_NE(attribute.as_path(), nullptr);
  EXPECT_EQ(*attribute.as_path(), Path {});

  ASSERT_NE(Const(attribute).as_path(), nullptr);
  EXPECT_EQ(*Const(attribute).as_path(), Path {});
}

TEST_F(AttributeTest, ResetToColor)
{
  Attribute attribute {};
  attribute.reset(AttributeKind::kColor);

  EXPECT_EQ(attribute.get_kind(), AttributeKind::kColor);
  EXPECT_EQ(attribute.has_default_value(), true);

  ASSERT_NE(attribute.as_color(), nullptr);
  EXPECT_EQ(*attribute.as_color(), Color {});

  ASSERT_NE(Const(attribute).as_color(), nullptr);
  EXPECT_EQ(*Const(attribute).as_color(), Color {});
}

TEST_F(AttributeTest, ResetToInvalidKind)
{
  Attribute attribute {};
  EXPECT_THROW(attribute.reset(AttributeKind {0xFFu}), std::invalid_argument);
}

}  // namespace
}  // namespace tactile
