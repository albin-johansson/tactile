// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/container/string.hpp"

#include <gtest/gtest.h>

using namespace tactile;

using int_literals::operator""_uz;

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
