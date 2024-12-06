// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/common/numeric/random.hpp"

#include <algorithm>  // generate, count
#include <array>      // array

#include <gtest/gtest.h>

namespace tactile::common {
namespace {

// tactile::common::random_int
TEST(Random, RandomInt)
{
  EXPECT_EQ(random_int(0, 0), 0);
  EXPECT_EQ(random_int(1, 1), 1);
  EXPECT_EQ(random_int(-1, -1), -1);

  const auto value = random_int(0, 10);
  EXPECT_GE(value, 0);
  EXPECT_LE(value, 10);
}

// tactile::common::random_uint
TEST(Random, RandomUInt)
{
  EXPECT_EQ(random_uint(0u, 0u), 0u);
  EXPECT_EQ(random_uint(1u, 1u), 1u);
  EXPECT_EQ(random_uint(~0u, ~0u), ~0u);

  const auto value = random_uint(0u, 10u);
  EXPECT_GE(value, 0u);
  EXPECT_LE(value, 10u);
}

// tactile::common::random_double
TEST(Random, RandomDouble)
{
  const auto value = random_double(-4.2, 83.1);
  EXPECT_GE(value, -4.2);
  EXPECT_LE(value, 83.1);
}

// tactile::common::random_bool
TEST(Random, GetRandomBool)
{
  std::array<bool, 2'000> values;
  std::ranges::generate(values, &random_bool);

  const auto true_count = std::ranges::count(values, true);
  const auto false_count = std::ssize(values) - true_count;
  // TACTILE_CORE_DEBUG("Called get_random_bool {} times: {} true, {} false",
  //                    values.size(),
  //                    true_count,
  //                    false_count);

  // This is far from perfect, but detects suspicious (unlikely) ratios.
  const auto ratio = static_cast<double>(true_count) / static_cast<double>(false_count);
  EXPECT_GT(ratio, 0.80);
  EXPECT_LT(ratio, 1.20);
}

}  // namespace
}  // namespace tactile::common
