// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/container/hash_map.hpp"

#include <string>
#include <string_view>

#include <gtest/gtest.h>

namespace tactile {
namespace {

using std::string_literals::operator""s;
using std::string_view_literals::operator""sv;

class HashMapTest : public testing::Test
{};

TEST_F(HashMapTest, HeterogeneousStringHashMapLookups)
{
  StringHashMap<int> map {};
  map["A"] = 12;
  map["B"] = 34;

  EXPECT_NE(map.find("A"), map.end());
  EXPECT_NE(map.find("A"s), map.end());
  EXPECT_NE(map.find("A"sv), map.end());

  EXPECT_NE(map.find("B"), map.end());
  EXPECT_NE(map.find("B"s), map.end());
  EXPECT_NE(map.find("B"sv), map.end());

  EXPECT_EQ(map.find("C"), map.end());
  EXPECT_EQ(map.find("C"s), map.end());
  EXPECT_EQ(map.find("C"sv), map.end());
}

}  // namespace
}  // namespace tactile
