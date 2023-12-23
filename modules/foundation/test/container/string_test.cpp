// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/container/string.hpp"

#include <limits>  // numeric_limits

#include <gtest/gtest.h>

#include "tactile/foundation/prelude.hpp"

using namespace tactile;
using namespace tactile::int_literals;

#if TACTILE_OS_WINDOWS
  #define NATIVE_STR(Str) L##Str
#else
  #define NATIVE_STR(Str) Str
#endif

/// \tests tactile::make_native_string
TEST(String, MakeNativeString)
{
  EXPECT_FALSE(make_native_string(nullptr).has_value());

  EXPECT_EQ(make_native_string(""), NATIVE_STR(""));
  EXPECT_EQ(make_native_string("1"), NATIVE_STR("1"));
  EXPECT_EQ(make_native_string("foo"), NATIVE_STR("foo"));
  EXPECT_EQ(make_native_string("bar.txt"), NATIVE_STR("bar.txt"));
  EXPECT_EQ(make_native_string("foo/bar"), NATIVE_STR("foo/bar"));
  EXPECT_EQ(make_native_string("foo/bar.txt"), NATIVE_STR("foo/bar.txt"));
  EXPECT_EQ(make_native_string("\0"), NATIVE_STR("\0"));
}

/// \tests tactile::str_split
TEST(String, StrSplit)
{
  const auto tokens = str_split("foo,bar,,x", ',');

  ASSERT_EQ(tokens.size(), 4_uz);
  EXPECT_EQ(tokens[0], "foo");
  EXPECT_EQ(tokens[1], "bar");
  EXPECT_EQ(tokens[2], "");
  EXPECT_EQ(tokens[3], "x");
}

/// \tests tactile::str_to_multiple_i32
TEST(String, StrToMultipleI32)
{
  EXPECT_TRUE(str_to_multiple_i32("", ' ').empty());
  EXPECT_TRUE(str_to_multiple_i32("/", '/').empty());
  EXPECT_TRUE(str_to_multiple_i32("!!", '!').empty());
  EXPECT_TRUE(str_to_multiple_i32("^0", '^').empty());
  EXPECT_TRUE(str_to_multiple_i32("1||", '|').empty());

  {
    const auto one = str_to_multiple_i32("42", '^');
    ASSERT_EQ(one.size(), 1_uz);
    EXPECT_EQ(one[0], 42);
  }

  {
    const auto two = str_to_multiple_i32("-3.7", '.');
    ASSERT_EQ(two.size(), 2_uz);
    EXPECT_EQ(two[0], -3);
    EXPECT_EQ(two[1], 7);
  }

  {
    const auto three = str_to_multiple_i32("1 2 3", ' ');
    ASSERT_EQ(three.size(), 3_uz);
    EXPECT_EQ(three[0], 1);
    EXPECT_EQ(three[1], 2);
    EXPECT_EQ(three[2], 3);
  }

  {
    const auto four = str_to_multiple_i32("10:20:30:40", ':');
    ASSERT_EQ(four.size(), 4_uz);
    EXPECT_EQ(four[0], 10);
    EXPECT_EQ(four[1], 20);
    EXPECT_EQ(four[2], 30);
    EXPECT_EQ(four[3], 40);
  }
}

/// \tests tactile::str_to_multiple_u32
TEST(String, StrToMultipleU32)
{
  EXPECT_TRUE(str_to_multiple_u32("", ' ').empty());
  EXPECT_TRUE(str_to_multiple_u32("/", '/').empty());
  EXPECT_TRUE(str_to_multiple_u32("!!", '!').empty());
  EXPECT_TRUE(str_to_multiple_u32("^0", '^').empty());
  EXPECT_TRUE(str_to_multiple_u32("1||", '|').empty());
  EXPECT_TRUE(str_to_multiple_u32("-1", ' ').empty());

  {
    const auto one = str_to_multiple_u32("42", '^');
    ASSERT_EQ(one.size(), 1_uz);
    EXPECT_EQ(one[0], 42u);
  }

  {
    const auto two = str_to_multiple_u32("3-7", '-');
    ASSERT_EQ(two.size(), 2_uz);
    EXPECT_EQ(two[0], 3u);
    EXPECT_EQ(two[1], 7u);
  }

  {
    const auto three = str_to_multiple_u32("1 2 3", ' ');
    ASSERT_EQ(three.size(), 3_uz);
    EXPECT_EQ(three[0], 1u);
    EXPECT_EQ(three[1], 2u);
    EXPECT_EQ(three[2], 3u);
  }

  {
    const auto four = str_to_multiple_u32("10:20:30:40", ':');
    ASSERT_EQ(four.size(), 4_uz);
    EXPECT_EQ(four[0], 10u);
    EXPECT_EQ(four[1], 20u);
    EXPECT_EQ(four[2], 30u);
    EXPECT_EQ(four[3], 40u);
  }
}

/// \tests tactile::str_to_multiple_f32
TEST(String, StrToMultipleF32)
{
  EXPECT_TRUE(str_to_multiple_f32("", ' ').empty());
  EXPECT_TRUE(str_to_multiple_f32("/", '/').empty());
  EXPECT_TRUE(str_to_multiple_f32("!!", '!').empty());
  EXPECT_TRUE(str_to_multiple_f32("^0.0", '^').empty());
  EXPECT_TRUE(str_to_multiple_f32("1.0||", '|').empty());

  {
    const auto one = str_to_multiple_f32("42", '^');
    ASSERT_EQ(one.size(), 1_uz);
    EXPECT_EQ(one[0], 42.0f);
  }

  {
    const auto two = str_to_multiple_f32("3.7@-100", '@');
    ASSERT_EQ(two.size(), 2_uz);
    EXPECT_EQ(two[0], 3.7f);
    EXPECT_EQ(two[1], -100.0f);
  }

  {
    const auto three = str_to_multiple_f32("1 2 3", ' ');
    ASSERT_EQ(three.size(), 3_uz);
    EXPECT_EQ(three[0], 1.0f);
    EXPECT_EQ(three[1], 2.0f);
    EXPECT_EQ(three[2], 3.0f);
  }

  {
    const auto four = str_to_multiple_f32("10:20:30:40", ':');
    ASSERT_EQ(four.size(), 4_uz);
    EXPECT_EQ(four[0], 10.0f);
    EXPECT_EQ(four[1], 20.0f);
    EXPECT_EQ(four[2], 30.0f);
    EXPECT_EQ(four[3], 40.0f);
  }
}

/// \tests tactile::str_to_u64
TEST(String, StrToU64)
{
  // Invalid inputs
  EXPECT_FALSE(str_to_u64("").has_value());
  EXPECT_FALSE(str_to_u64(" ").has_value());
  EXPECT_FALSE(str_to_u64("42 ").has_value());
  EXPECT_FALSE(str_to_u64("foo42").has_value());
  EXPECT_FALSE(str_to_u64("42foo").has_value());
  EXPECT_FALSE(str_to_u64("-92").has_value());
  EXPECT_FALSE(str_to_u64("+42").has_value());
  EXPECT_FALSE(str_to_u64("A").has_value());
  EXPECT_FALSE(str_to_u64("2", 2).has_value());
  EXPECT_FALSE(str_to_u64("9", 8).has_value());
  EXPECT_FALSE(str_to_u64("G", 16).has_value());
  EXPECT_FALSE(str_to_u64("0xFF", 16).has_value());

  // Valid inputs
  EXPECT_EQ(str_to_u64("42"), uint64 {42});
  EXPECT_EQ(str_to_u64("042"), uint64 {42});
  EXPECT_EQ(str_to_u64("111", 2), uint64 {0b111});
  EXPECT_EQ(str_to_u64("A", 16), uint64 {0xA});
  EXPECT_EQ(str_to_u64("F", 16), uint64 {0xF});
  EXPECT_EQ(str_to_u64("10", 8), uint64 {010});
  EXPECT_EQ(str_to_u64("FFFFFFFFFFFFFFFF", 16), std::numeric_limits<uint64>::max());
  EXPECT_EQ(str_to_u64(StringView {"1234"}.substr(0, 2)), uint64 {12});
  EXPECT_EQ(str_to_u64(StringView {"1234"}.substr(2, 2)), uint64 {34});
}

/// \tests tactile::str_to_i64
TEST(String, StrToI64)
{
  // Invalid inputs
  EXPECT_FALSE(str_to_i64("").has_value());
  EXPECT_FALSE(str_to_i64(" ").has_value());
  EXPECT_FALSE(str_to_i64("42 ").has_value());
  EXPECT_FALSE(str_to_i64("foo42").has_value());
  EXPECT_FALSE(str_to_i64("42foo").has_value());
  EXPECT_FALSE(str_to_i64("+42").has_value());
  EXPECT_FALSE(str_to_i64("A").has_value());
  EXPECT_FALSE(str_to_i64("2", 2).has_value());
  EXPECT_FALSE(str_to_i64("9", 8).has_value());
  EXPECT_FALSE(str_to_i64("G", 16).has_value());
  EXPECT_FALSE(str_to_i64("0xFF", 16).has_value());

  // Valid inputs
  EXPECT_EQ(str_to_i64("0"), int64 {0});
  EXPECT_EQ(str_to_i64("-182"), int64 {-182});
  EXPECT_EQ(str_to_i64("99"), int64 {99});
  EXPECT_EQ(str_to_i64("027"), int64 {27});
  EXPECT_EQ(str_to_i64("1000", 2), int64 {0b1000});
  EXPECT_EQ(str_to_i64("A", 16), int64 {0xA});
  EXPECT_EQ(str_to_i64("F", 16), int64 {0xF});
  EXPECT_EQ(str_to_i64("10", 8), int64 {010});
  EXPECT_EQ(str_to_i64("7FFFFFFFFFFFFFFF", 16), std::numeric_limits<int64>::max());
  EXPECT_EQ(str_to_i64(StringView {"1234"}.substr(0, 2)), int64 {12});
  EXPECT_EQ(str_to_i64(StringView {"1234"}.substr(2, 2)), int64 {34});
}

/// \tests tactile::str_to_f64
TEST(String, StrToF64)
{
  // Invalid inputs
  EXPECT_FALSE(str_to_f64("").has_value());
  EXPECT_FALSE(str_to_f64(" ").has_value());
  EXPECT_FALSE(str_to_f64(".").has_value());
  EXPECT_FALSE(str_to_f64("4.2 ").has_value());
  EXPECT_FALSE(str_to_f64("foo9.0").has_value());
  EXPECT_FALSE(str_to_f64("40.92foo").has_value());
  EXPECT_FALSE(str_to_f64("+4.5").has_value());

  // Valid inputs
  EXPECT_EQ(str_to_f64("0"), 0.0);
  EXPECT_EQ(str_to_f64("42"), 42.0);
  EXPECT_EQ(str_to_f64("5."), 5.0);
  EXPECT_EQ(str_to_f64(".1"), 0.1);
  EXPECT_EQ(str_to_f64("0.1"), 0.1);
  EXPECT_EQ(str_to_f64("1.0"), 1.0);
  EXPECT_EQ(str_to_f64("3.2"), 3.2);
  EXPECT_EQ(str_to_f64("-1.0"), -1.0);
  EXPECT_EQ(str_to_f64(StringView {"1234"}.substr(0, 2)), 12.0);
  EXPECT_EQ(str_to_f64(StringView {"1234"}.substr(2, 2)), 34.0);
}
