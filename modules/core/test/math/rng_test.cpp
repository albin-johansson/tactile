/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tactile/core/math/rng.hpp"

#include <algorithm>  // generate, count
#include <iostream>   // cout

#include <gtest/gtest.h>

#include "tactile/core/type/array.hpp"

using namespace tactile;

TEST(RNG, next_random_i32)
{
  EXPECT_EQ(next_random_i32(0, 0), 0);
  EXPECT_EQ(next_random_i32(1, 1), 1);
  EXPECT_EQ(next_random_i32(-1, -1), -1);

  const auto value = next_random_u32(0u, 10u);
  EXPECT_GE(value, 0u);
  EXPECT_LE(value, 10u);
}

TEST(RNG, next_random_f32)
{
  const auto value = next_random_f32(0.0f, 1.0f);
  EXPECT_GE(value, 0.0f);
  EXPECT_LE(value, 1.0f);
}

TEST(RNG, next_bool)
{
  Array<bool, 2'000> values;
  std::ranges::generate(values, &next_bool);

  const auto true_count = std::ranges::count(values, true);
  const auto false_count = std::ssize(values) - true_count;
  std::cout << "[RNG] " << values.size() << " next_bool invocations: "  //
            << true_count << " true, " << false_count << " false\n";

  // This is far from perfect, but detects suspicious (unlikely) ratios.
  const auto ratio = static_cast<double>(true_count) / static_cast<double>(false_count);
  EXPECT_GT(ratio, 0.90);
  EXPECT_LT(ratio, 1.10);
}

TEST(RNG, next_float)
{
  const auto value = next_float();
  EXPECT_GT(value, 0.0f);
  EXPECT_LT(value, 1.0f);
}