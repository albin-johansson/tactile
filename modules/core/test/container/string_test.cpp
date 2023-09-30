// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/container/string.hpp"

#include <gtest/gtest.h>

#include "tactile/core/prelude.hpp"

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

/// \tests tactile::str_to_u32
TEST(String, StrToU32)
{
  // Invalid inputs
  EXPECT_EQ(str_to_u32(""), kNone);
  EXPECT_EQ(str_to_u32(" "), kNone);
  EXPECT_EQ(str_to_u32("42 "), kNone);
  EXPECT_EQ(str_to_u32("foo42"), kNone);
  EXPECT_EQ(str_to_u32("42foo"), kNone);
  EXPECT_EQ(str_to_u32("-92"), kNone);
  EXPECT_EQ(str_to_u32("+42"), kNone);
  EXPECT_EQ(str_to_u32("A"), kNone);
  EXPECT_EQ(str_to_u32("2", 2), kNone);
  EXPECT_EQ(str_to_u32("9", 8), kNone);
  EXPECT_EQ(str_to_u32("G", 16), kNone);
  EXPECT_EQ(str_to_u32("0xFF", 16), kNone);

  // Valid inputs
  EXPECT_EQ(str_to_u32("42"), uint32 {42});
  EXPECT_EQ(str_to_u32("042"), uint32 {42});
  EXPECT_EQ(str_to_u32("111", 2), uint32 {0b111});
  EXPECT_EQ(str_to_u32("A", 16), uint32 {0xA});
  EXPECT_EQ(str_to_u32("F", 16), uint32 {0xF});
  EXPECT_EQ(str_to_u32("10", 8), uint32 {010});
  EXPECT_EQ(str_to_u32(StringView {"1234"}.substr(0, 2)), uint32 {12});
  EXPECT_EQ(str_to_u32(StringView {"1234"}.substr(2, 2)), uint32 {34});
}

/// \tests tactile::str_to_i32
TEST(String, StrToI32)
{
  // Invalid inputs
  EXPECT_EQ(str_to_i32(""), kNone);
  EXPECT_EQ(str_to_i32(" "), kNone);
  EXPECT_EQ(str_to_i32("42 "), kNone);
  EXPECT_EQ(str_to_i32("foo42"), kNone);
  EXPECT_EQ(str_to_i32("42foo"), kNone);
  EXPECT_EQ(str_to_i32("+42"), kNone);
  EXPECT_EQ(str_to_i32("A"), kNone);
  EXPECT_EQ(str_to_i32("2", 2), kNone);
  EXPECT_EQ(str_to_i32("9", 8), kNone);
  EXPECT_EQ(str_to_i32("G", 16), kNone);
  EXPECT_EQ(str_to_i32("0xFF", 16), kNone);

  // Valid inputs
  EXPECT_EQ(str_to_i32("0"), int32 {0});
  EXPECT_EQ(str_to_i32("-182"), int32 {-182});
  EXPECT_EQ(str_to_i32("99"), int32 {99});
  EXPECT_EQ(str_to_i32("027"), int32 {27});
  EXPECT_EQ(str_to_i32("1000", 2), int32 {0b1000});
  EXPECT_EQ(str_to_i32("A", 16), int32 {0xA});
  EXPECT_EQ(str_to_i32("F", 16), int32 {0xF});
  EXPECT_EQ(str_to_i32("10", 8), int32 {010});
  EXPECT_EQ(str_to_i32(StringView {"1234"}.substr(0, 2)), int32 {12});
  EXPECT_EQ(str_to_i32(StringView {"1234"}.substr(2, 2)), int32 {34});
}

/// \tests tactile::str_to_f32
TEST(String, StrToF32)
{
  // Invalid inputs
  EXPECT_EQ(str_to_f32(""), kNone);
  EXPECT_EQ(str_to_f32(" "), kNone);
  EXPECT_EQ(str_to_f32("."), kNone);
  EXPECT_EQ(str_to_f32("4.2 "), kNone);
  EXPECT_EQ(str_to_f32("foo9.0"), kNone);
  EXPECT_EQ(str_to_f32("40.92foo"), kNone);
  EXPECT_EQ(str_to_f32("+4.5"), kNone);

  // Valid inputs
  EXPECT_EQ(str_to_f32("0"), 0.0f);
  EXPECT_EQ(str_to_f32("42"), 42.0f);
  EXPECT_EQ(str_to_f32("5."), 5.0f);
  EXPECT_EQ(str_to_f32(".1"), 0.1f);
  EXPECT_EQ(str_to_f32("0.1"), 0.1f);
  EXPECT_EQ(str_to_f32("1.0"), 1.0f);
  EXPECT_EQ(str_to_f32("3.2"), 3.2f);
  EXPECT_EQ(str_to_f32("-1.0"), -1.0f);
  EXPECT_EQ(str_to_f32(StringView {"1234"}.substr(0, 2)), 12.0f);
  EXPECT_EQ(str_to_f32(StringView {"1234"}.substr(2, 2)), 34.0f);
}
