// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/math/rng.hpp"

#include <algorithm>  // generate, count
#include <iostream>   // cout

#include <gtest/gtest.h>

#include "tactile/core/container/array.hpp"

using namespace tactile;

/// \tests tactile::next_random_i32
TEST(RNG, NextRandomI32)
{
  EXPECT_EQ(next_random_i32(0, 0), 0);
  EXPECT_EQ(next_random_i32(1, 1), 1);
  EXPECT_EQ(next_random_i32(-1, -1), -1);

  const auto value = next_random_u32(0u, 10u);
  EXPECT_GE(value, 0u);
  EXPECT_LE(value, 10u);
}

/// \tests tactile::next_random_f32
TEST(RNG, NextRandomF32)
{
  const auto value = next_random_f32(0.0f, 1.0f);
  EXPECT_GE(value, 0.0f);
  EXPECT_LE(value, 1.0f);
}

/// \tests tactile::next_bool
TEST(RNG, NextBool)
{
  Array<bool, 2'000> values;
  std::ranges::generate(values, &next_bool);

  const auto true_count = std::ranges::count(values, true);
  const auto false_count = std::ssize(values) - true_count;
  std::cout << "[RNG] " << values.size() << " next_bool invocations: "  //
            << true_count << " true, " << false_count << " false\n";

  // This is far from perfect, but detects suspicious (unlikely) ratios.
  const auto ratio = static_cast<double>(true_count) / static_cast<double>(false_count);
  EXPECT_GT(ratio, 0.80);
  EXPECT_LT(ratio, 1.20);
}

/// \tests tactile::next_normalized_float
TEST(RNG, NextNormalizedFloat)
{
  const auto value = next_normalized_float();
  EXPECT_GT(value, 0.0f);
  EXPECT_LT(value, 1.0f);
}