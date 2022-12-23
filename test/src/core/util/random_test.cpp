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

#include "core/util/random.hpp"

#include <algorithm>  // generate, count

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "core/type/array.hpp"

namespace tactile::test {

TEST(Random, NextRandomInt)
{
  ASSERT_EQ(0, next_random_i32(0, 0));
  ASSERT_EQ(1, next_random_i32(1, 1));
  ASSERT_EQ(-1, next_random_i32(-1, -1));

  const auto value = next_random_u32(0u, 100u);
  ASSERT_GE(value, 0u);
  ASSERT_LE(value, 100u);
}

TEST(Random, NextRandomFloat)
{
  const auto value = next_random_f32(0.0f, 1.0f);
  ASSERT_GE(value, 0.0f);
  ASSERT_LE(value, 1.0f);
}

TEST(Random, NextBool)
{
  Array<bool, 1'000> values;
  std::generate(values.begin(), values.end(), next_bool);

  const auto true_count = std::count(values.begin(), values.end(), true);
  const auto false_count = static_cast<diff_t>(values.size()) - true_count;

  spdlog::debug("[Random] {} next_bool invocations: {} true, {} false",
                values.size(),
                true_count,
                false_count);
}

TEST(Random, NextFloat)
{
  const auto value = next_float();
  ASSERT_GE(value, 0.0f);
  ASSERT_LE(value, 1.0f);
}

}  // namespace tactile::test