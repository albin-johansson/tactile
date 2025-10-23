// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/container/lookup.hpp"

#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include <gtest/gtest.h>

#include "tactile/core/container/hash_map.hpp"

namespace tactile {
namespace {

using std::string_literals::operator""s;
using std::string_view_literals::operator""sv;

class LookupTest : public testing::Test
{
 public:
  void SetUp() override
  {
    m_map["A"] = 1;
    m_map["B"] = 2;
    m_map["C"] = 3;
  }

 protected:
  StringHashMap<int> m_map {};
};

TEST_F(LookupTest, TryGetFrom)
{
  const auto check = [this](const auto& key, const int expected_value) {
    {
      const auto* const value_ptr = try_get_from(m_map, key);
      ASSERT_NE(value_ptr, nullptr);
      EXPECT_EQ(*value_ptr, expected_value);
    }

    {
      const auto* const value_ptr = try_get_from(std::as_const(m_map), key);
      ASSERT_NE(value_ptr, nullptr);
      EXPECT_EQ(*value_ptr, expected_value);
    }
  };

  check("A", 1);
  check("B", 2);
  check("C", 3);
  check("C"s, 3);
  check("C"sv, 3);

  EXPECT_EQ(try_get_from(m_map, "D"), nullptr);
  EXPECT_EQ(try_get_from(std::as_const(m_map), "D"), nullptr);
}

TEST_F(LookupTest, GetFrom)
{
  const auto check = [this](const auto& key, const int expected_value) {
    EXPECT_EQ(get_from(m_map, key), expected_value);
    EXPECT_EQ(get_from(std::as_const(m_map), key), expected_value);
  };

  check("A", 1);
  check("B", 2);
  check("C", 3);
  check("C"s, 3);
  check("C"sv, 3);

  EXPECT_THROW(get_from(m_map, "D"), std::invalid_argument);
  EXPECT_THROW(get_from(std::as_const(m_map), "D"), std::invalid_argument);
}

TEST_F(LookupTest, ExtractFrom)
{
  EXPECT_EQ(extract_from(m_map, "A"), 1);
  EXPECT_EQ(extract_from(m_map, "B"), 2);
  EXPECT_EQ(extract_from(m_map, "C"), 3);
  EXPECT_EQ(extract_from(m_map, "D"), kNone);
}

}  // namespace
}  // namespace tactile
