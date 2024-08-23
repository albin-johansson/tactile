// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/base/io/int_parser.hpp"

#include <string_view>  // string_view

#include <gtest/gtest.h>

#include "tactile/base/int.hpp"

namespace tactile::test {

// tactile::parse [integral]
TEST(StringConv, ParseInvalidInt)
{
  EXPECT_FALSE(parse<int64>("").has_value());
  EXPECT_FALSE(parse<int64>(" ").has_value());
  EXPECT_FALSE(parse<int64>("42 ").has_value());
  EXPECT_FALSE(parse<int64>("foo42").has_value());
  EXPECT_FALSE(parse<int64>("42foo").has_value());
  EXPECT_FALSE(parse<int64>("+42").has_value());
  EXPECT_FALSE(parse<int64>("A").has_value());
  EXPECT_FALSE(parse<int64>("2", 2).has_value());
  EXPECT_FALSE(parse<int64>("9", 8).has_value());
  EXPECT_FALSE(parse<int64>("G", 16).has_value());
  EXPECT_FALSE(parse<int64>("0xFF", 16).has_value());
}

// tactile::parse [integral]
TEST(StringConv, ParseValidInt)
{
  EXPECT_EQ(parse<int64>("0"), int64 {0});
  EXPECT_EQ(parse<int64>("-182"), int64 {-182});
  EXPECT_EQ(parse<int64>("99"), int64 {99});
  EXPECT_EQ(parse<int64>("027"), int64 {27});
  EXPECT_EQ(parse<int64>("1000", 2), int64 {0b1000});
  EXPECT_EQ(parse<int64>("A", 16), int64 {0xA});
  EXPECT_EQ(parse<int64>("F", 16), int64 {0xF});
  EXPECT_EQ(parse<int64>("10", 8), int64 {010});
  EXPECT_EQ(parse<int64>("7FFFFFFFFFFFFFFF", 16), std::numeric_limits<int64>::max());
  EXPECT_EQ(parse<int64>(std::string_view {"1234"}.substr(0, 2)), int64 {12});
  EXPECT_EQ(parse<int64>(std::string_view {"1234"}.substr(2, 2)), int64 {34});
}

// tactile::parse [integral]
TEST(StringConv, ParseInvalidUint)
{
  EXPECT_FALSE(parse<uint64>("").has_value());
  EXPECT_FALSE(parse<uint64>(" ").has_value());
  EXPECT_FALSE(parse<uint64>("42 ").has_value());
  EXPECT_FALSE(parse<uint64>("foo42").has_value());
  EXPECT_FALSE(parse<uint64>("42foo").has_value());
  EXPECT_FALSE(parse<uint64>("-92").has_value());
  EXPECT_FALSE(parse<uint64>("+42").has_value());
  EXPECT_FALSE(parse<uint64>("A").has_value());
  EXPECT_FALSE(parse<uint64>("2", 2).has_value());
  EXPECT_FALSE(parse<uint64>("9", 8).has_value());
  EXPECT_FALSE(parse<uint64>("G", 16).has_value());
  EXPECT_FALSE(parse<uint64>("0xFF", 16).has_value());
}

// tactile::parse [integral]
TEST(StringConv, ParseValidUint)
{
  EXPECT_EQ(parse<uint64>("42"), uint64 {42});
  EXPECT_EQ(parse<uint64>("042"), uint64 {42});
  EXPECT_EQ(parse<uint64>("111", 2), uint64 {0b111});
  EXPECT_EQ(parse<uint64>("A", 16), uint64 {0xA});
  EXPECT_EQ(parse<uint64>("F", 16), uint64 {0xF});
  EXPECT_EQ(parse<uint64>("10", 8), uint64 {010});
  EXPECT_EQ(parse<uint64>("FFFFFFFFFFFFFFFF", 16), std::numeric_limits<uint64>::max());
  EXPECT_EQ(parse<uint64>(std::string_view {"1234"}.substr(0, 2)), uint64 {12});
  EXPECT_EQ(parse<uint64>(std::string_view {"1234"}.substr(2, 2)), uint64 {34});
}

}  // namespace tactile::test
