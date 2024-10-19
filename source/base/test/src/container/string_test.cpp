// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/base/container/string.hpp"

#include <vector>  // vector

#include <gtest/gtest.h>

namespace tactile {
namespace {

// tactile::visit_tokens
TEST(String, VisitTokensEmpty)
{
  std::vector<std::string> tokens {};
  visit_tokens("", '!', [&tokens](const std::string_view token) {
    tokens.emplace_back(token);
    return true;
  });

  EXPECT_EQ(tokens.size(), 0);
}

// tactile::visit_tokens
TEST(String, VisitTokensLetters)
{
  std::vector<std::string> tokens {};
  visit_tokens("a:b:c:d", ':', [&tokens](const std::string_view token) {
    tokens.emplace_back(token);
    return true;
  });

  ASSERT_EQ(tokens.size(), 4);
  EXPECT_EQ(tokens.at(0), "a");
  EXPECT_EQ(tokens.at(1), "b");
  EXPECT_EQ(tokens.at(2), "c");
  EXPECT_EQ(tokens.at(3), "d");
}

// tactile::visit_tokens
TEST(String, VisitTokensNumbers)
{
  std::vector<std::string> tokens {};
  visit_tokens("1 200 30 4000", ' ', [&tokens](const std::string_view token) {
    tokens.emplace_back(token);
    return true;
  });

  ASSERT_EQ(tokens.size(), 4);
  EXPECT_EQ(tokens.at(0), "1");
  EXPECT_EQ(tokens.at(1), "200");
  EXPECT_EQ(tokens.at(2), "30");
  EXPECT_EQ(tokens.at(3), "4000");
}

// tactile::visit_tokens
TEST(String, VisitTokensWithLeadingSeparator)
{
  std::vector<std::string> tokens {};
  visit_tokens(".woah", '.', [&tokens](const std::string_view token) {
    tokens.emplace_back(token);
    return true;
  });

  ASSERT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens.at(0), "");
  EXPECT_EQ(tokens.at(1), "woah");
}

// tactile::visit_tokens
TEST(String, VisitTokensWithTrailingSeparator)
{
  std::vector<std::string> tokens {};
  visit_tokens("foobar!", '!', [&tokens](const std::string_view token) {
    tokens.emplace_back(token);
    return true;
  });

  ASSERT_EQ(tokens.size(), 1);
  EXPECT_EQ(tokens.at(0), "foobar");
}

// tactile::visit_tokens
TEST(String, VisitTokensWithEmptyTokens)
{
  std::vector<std::string> tokens {};
  visit_tokens("aaa::a:bb::c", ':', [&tokens](const std::string_view token) {
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

// tactile::trim_string
TEST(String, TrimString)
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

}  // namespace
}  // namespace tactile
