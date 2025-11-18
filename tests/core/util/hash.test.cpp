// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/util/hash.hpp"

#include <functional>

#include <gtest/gtest.h>

#include "tactile/core/container/string.hpp"

namespace tactile {
namespace {

struct CustomType final
{
  int a {};
  float b {};
};

}  // namespace
}  // namespace tactile

TACTILE_IMPL_HASH(tactile::CustomType, value.a, value.b);

namespace tactile {
namespace {

class HashTest : public testing::Test
{};

TEST_F(HashTest, HashCombine)
{
  EXPECT_EQ(hash_combine(), 0uz);
  EXPECT_NE(hash_combine(0), 0uz);
  EXPECT_NE(hash_combine(1, 2, 3), 0uz);
  EXPECT_NE(hash_combine(1.0f, 2.0f), 0uz);
  EXPECT_NE(hash_combine(StringView {"foo"}, StringView {"foo"}), 0uz);
}

TEST_F(HashTest, ImplHashMacro)
{
  constexpr CustomType value {.a = 42, .b = 1.5f};
  EXPECT_EQ(std::hash<CustomType> {}(value), hash_combine(value.a, value.b));
}

}  // namespace
}  // namespace tactile
