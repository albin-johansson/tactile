// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/string_conv.hpp"

#include <limits>  // numeric_limits

#include <gtest/gtest.h>

namespace tactile {

/** \trace tactile::parse_int */
TEST(StringConv, ParseInt)
{
  // Invalid inputs
  EXPECT_FALSE(parse_int("").has_value());
  EXPECT_FALSE(parse_int(" ").has_value());
  EXPECT_FALSE(parse_int("42 ").has_value());
  EXPECT_FALSE(parse_int("foo42").has_value());
  EXPECT_FALSE(parse_int("42foo").has_value());
  EXPECT_FALSE(parse_int("+42").has_value());
  EXPECT_FALSE(parse_int("A").has_value());
  EXPECT_FALSE(parse_int("2", 2).has_value());
  EXPECT_FALSE(parse_int("9", 8).has_value());
  EXPECT_FALSE(parse_int("G", 16).has_value());
  EXPECT_FALSE(parse_int("0xFF", 16).has_value());

  // Valid inputs
  EXPECT_EQ(parse_int("0"), int64 {0});
  EXPECT_EQ(parse_int("-182"), int64 {-182});
  EXPECT_EQ(parse_int("99"), int64 {99});
  EXPECT_EQ(parse_int("027"), int64 {27});
  EXPECT_EQ(parse_int("1000", 2), int64 {0b1000});
  EXPECT_EQ(parse_int("A", 16), int64 {0xA});
  EXPECT_EQ(parse_int("F", 16), int64 {0xF});
  EXPECT_EQ(parse_int("10", 8), int64 {010});
  EXPECT_EQ(parse_int("7FFFFFFFFFFFFFFF", 16), std::numeric_limits<int64>::max());
  EXPECT_EQ(parse_int(StringView {"1234"}.substr(0, 2)), int64 {12});
  EXPECT_EQ(parse_int(StringView {"1234"}.substr(2, 2)), int64 {34});
}

/** \trace tactile::parse_uint */
TEST(StringConv, ParseUint)
{
  // Invalid inputs
  EXPECT_FALSE(parse_uint("").has_value());
  EXPECT_FALSE(parse_uint(" ").has_value());
  EXPECT_FALSE(parse_uint("42 ").has_value());
  EXPECT_FALSE(parse_uint("foo42").has_value());
  EXPECT_FALSE(parse_uint("42foo").has_value());
  EXPECT_FALSE(parse_uint("-92").has_value());
  EXPECT_FALSE(parse_uint("+42").has_value());
  EXPECT_FALSE(parse_uint("A").has_value());
  EXPECT_FALSE(parse_uint("2", 2).has_value());
  EXPECT_FALSE(parse_uint("9", 8).has_value());
  EXPECT_FALSE(parse_uint("G", 16).has_value());
  EXPECT_FALSE(parse_uint("0xFF", 16).has_value());

  // Valid inputs
  EXPECT_EQ(parse_uint("42"), uint64 {42});
  EXPECT_EQ(parse_uint("042"), uint64 {42});
  EXPECT_EQ(parse_uint("111", 2), uint64 {0b111});
  EXPECT_EQ(parse_uint("A", 16), uint64 {0xA});
  EXPECT_EQ(parse_uint("F", 16), uint64 {0xF});
  EXPECT_EQ(parse_uint("10", 8), uint64 {010});
  EXPECT_EQ(parse_uint("FFFFFFFFFFFFFFFF", 16), std::numeric_limits<uint64>::max());
  EXPECT_EQ(parse_uint(StringView {"1234"}.substr(0, 2)), uint64 {12});
  EXPECT_EQ(parse_uint(StringView {"1234"}.substr(2, 2)), uint64 {34});
}

/** \trace tactile::parse_float */
TEST(StringConv, ParseFloat)
{
  // Invalid inputs
  EXPECT_FALSE(parse_float("").has_value());
  EXPECT_FALSE(parse_float(" ").has_value());
  EXPECT_FALSE(parse_float(".").has_value());
  EXPECT_FALSE(parse_float("4.2 ").has_value());
  EXPECT_FALSE(parse_float("foo9.0").has_value());
  EXPECT_FALSE(parse_float("40.92foo").has_value());
  EXPECT_FALSE(parse_float("+4.5").has_value());

  // Valid inputs
  EXPECT_EQ(parse_float("0"), 0.0f);
  EXPECT_EQ(parse_float("42"), 42.0f);
  EXPECT_EQ(parse_float("5."), 5.0f);
  EXPECT_EQ(parse_float(".1"), 0.1f);
  EXPECT_EQ(parse_float("1.0"), 1.0f);
  EXPECT_EQ(parse_float("3.2"), 3.2f);
  EXPECT_EQ(parse_float("-1.0"), -1.0f);
  EXPECT_EQ(parse_float(StringView {"1234"}.substr(0, 2)), 12.0f);
  EXPECT_EQ(parse_float(StringView {"1234"}.substr(2, 2)), 34.0f);
}

}  // namespace tactile
