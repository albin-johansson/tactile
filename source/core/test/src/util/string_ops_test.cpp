// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/string_ops.hpp"

#include <iostream>

#include <gtest/gtest.h>

#include "tactile/base/container/vector.hpp"

namespace tactile {

/** \trace tactile::split_string */
TEST(StringOps, SplitStringEmpty)
{
  Vector<String> tokens {};
  split_string("", '!', [&tokens](const StringView token) {
    tokens.emplace_back(token);
    return true;
  });

  EXPECT_EQ(tokens.size(), 0);
}

/** \trace tactile::split_string */
TEST(StringOps, SplitStringLetters)
{
  Vector<String> tokens {};
  split_string("a:b:c:d", ':', [&tokens](const StringView token) {
    tokens.emplace_back(token);
    return true;
  });

  ASSERT_EQ(tokens.size(), 4);
  EXPECT_EQ(tokens.at(0), "a");
  EXPECT_EQ(tokens.at(1), "b");
  EXPECT_EQ(tokens.at(2), "c");
  EXPECT_EQ(tokens.at(3), "d");
}

/** \trace tactile::split_string */
TEST(StringOps, SplitStringNumbers)
{
  Vector<String> tokens {};
  split_string("1 200 30 4000", ' ', [&tokens](const StringView token) {
    tokens.emplace_back(token);
    return true;
  });

  ASSERT_EQ(tokens.size(), 4);
  EXPECT_EQ(tokens.at(0), "1");
  EXPECT_EQ(tokens.at(1), "200");
  EXPECT_EQ(tokens.at(2), "30");
  EXPECT_EQ(tokens.at(3), "4000");
}

/** \trace tactile::split_string */
TEST(StringOps, SplitStringWithLeadingSeparator)
{
  Vector<String> tokens {};
  split_string(".woah", '.', [&tokens](const StringView token) {
    tokens.emplace_back(token);
    return true;
  });

  ASSERT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens.at(0), "");
  EXPECT_EQ(tokens.at(1), "woah");
}

/** \trace tactile::split_string */
TEST(StringOps, SplitStringWithTrailingSeparator)
{
  Vector<String> tokens {};
  split_string("foobar!", '!', [&tokens](const StringView token) {
    tokens.emplace_back(token);
    return true;
  });

  ASSERT_EQ(tokens.size(), 1);
  EXPECT_EQ(tokens.at(0), "foobar");
}

/** \trace tactile::split_string */
TEST(StringOps, SplitStringWithEmptyTokens)
{
  Vector<String> tokens {};
  split_string("aaa::a:bb::c", ':', [&tokens](const StringView token) {
    tokens.emplace_back(token);
    return true;
  });

  ASSERT_EQ(tokens.size(), 6);
  EXPECT_EQ(tokens.at(0), "aaa");
  EXPECT_EQ(tokens.at(1), "");
  EXPECT_EQ(tokens.at(2), "a");
  EXPECT_EQ(tokens.at(3), "bb");
  EXPECT_EQ(tokens.at(4), "");
  EXPECT_EQ(tokens.at(5), "c");
}

/** \trace tactile::trim_string */
TEST(StringOps, TrimString)
{
  EXPECT_EQ(trim_string(" "), " ");
  EXPECT_EQ(trim_string("  "), "  ");
  EXPECT_EQ(trim_string(" x "), "x");
  EXPECT_EQ(trim_string("foobar"), "foobar");
  EXPECT_EQ(trim_string("foobar "), "foobar");
  EXPECT_EQ(trim_string(" foobar"), "foobar");
  EXPECT_EQ(trim_string(" foobar "), "foobar");
  EXPECT_EQ(trim_string(" foo bar "), "foo bar");
  EXPECT_EQ(trim_string("   a   b c   d      "), "a   b c   d");
}

}  // namespace tactile
