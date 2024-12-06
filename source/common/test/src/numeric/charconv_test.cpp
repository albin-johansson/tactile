// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/common/numeric/charconv.hpp"

#include <cstdint>
#include <limits>
#include <string_view>

#include <gtest/gtest.h>

namespace tactile::common {
namespace {

// tactile::common::parse_int
TEST(NumericCharConv, ParseInt)
{
  EXPECT_EQ(parse_int("0"), std::int64_t {0});
  EXPECT_EQ(parse_int("-182"), std::int64_t {-182});
  EXPECT_EQ(parse_int("99"), std::int64_t {99});
  EXPECT_EQ(parse_int("027"), std::int64_t {27});
  EXPECT_EQ(parse_int("1000", 2), std::int64_t {0b1000});
  EXPECT_EQ(parse_int("A", 16), std::int64_t {0xA});
  EXPECT_EQ(parse_int("F", 16), std::int64_t {0xF});
  EXPECT_EQ(parse_int("10", 8), std::int64_t {010});
  EXPECT_EQ(parse_int("7FFFFFFFFFFFFFFF", 16), std::numeric_limits<std::int64_t>::max());
  EXPECT_EQ(parse_int(std::string_view {"1234"}.substr(0, 2)), std::int64_t {12});
  EXPECT_EQ(parse_int(std::string_view {"1234"}.substr(2, 2)), std::int64_t {34});
}

// tactile::common::parse_int
TEST(NumericCharConv, ParseIntErrors)
{
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
}

// tactile::common::parse_uint
TEST(NumericCharConv, ParseUIntErrors)
{
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
}

// tactile::common::parse_uint
TEST(NumericCharConv, ParseUInt)
{
  EXPECT_EQ(parse_uint("42"), std::uint64_t {42});
  EXPECT_EQ(parse_uint("042"), std::uint64_t {42});
  EXPECT_EQ(parse_uint("111", 2), std::uint64_t {0b111});
  EXPECT_EQ(parse_uint("A", 16), std::uint64_t {0xA});
  EXPECT_EQ(parse_uint("F", 16), std::uint64_t {0xF});
  EXPECT_EQ(parse_uint("10", 8), std::uint64_t {010});
  EXPECT_EQ(parse_uint("FFFFFFFFFFFFFFFF", 16), std::numeric_limits<std::uint64_t>::max());
  EXPECT_EQ(parse_uint(std::string_view {"1234"}.substr(0, 2)), std::uint64_t {12});
  EXPECT_EQ(parse_uint(std::string_view {"1234"}.substr(2, 2)), std::uint64_t {34});
}

}  // namespace
}  // namespace tactile::common
