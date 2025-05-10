// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

import std;
import tactile.core;

namespace tactile::tests {
namespace {

class StringOpsTest : public testing::Test
{};

TEST_F(StringOpsTest, ParseU64)
{
  EXPECT_EQ(parse_u64("42"), 42U);

  EXPECT_EQ(parse_u64("0"), std::numeric_limits<u64>::min());
  EXPECT_FALSE(parse_u64("-1").has_value());

  EXPECT_EQ(parse_u64("18446744073709551615"), std::numeric_limits<u64>::max());
  EXPECT_FALSE(parse_u64("18446744073709551616").has_value());

  EXPECT_FALSE(parse_u64("foobar").has_value());
}

TEST_F(StringOpsTest, ParseI64)
{
  EXPECT_EQ(parse_i64("42"), 42);
  EXPECT_EQ(parse_i64("-123"), -123);

  EXPECT_EQ(parse_i64("-9223372036854775808"), std::numeric_limits<i64>::min());
  EXPECT_FALSE(parse_i64("-9223372036854775809").has_value());

  EXPECT_EQ(parse_i64("9223372036854775807"), std::numeric_limits<i64>::max());
  EXPECT_FALSE(parse_i64("9223372036854775808").has_value());

  EXPECT_FALSE(parse_i64("foobar").has_value());
}

TEST_F(StringOpsTest, ParseF64)
{
  EXPECT_EQ(parse_f64("0"), 0.0);
  EXPECT_EQ(parse_f64("4.2"), 4.2);
  EXPECT_EQ(parse_f64("-10"), -10.0);

  EXPECT_FALSE(parse_f64("foobar").has_value());
}

}  // namespace
}  // namespace tactile::tests
