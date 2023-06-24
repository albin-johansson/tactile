/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "common/util/random.hpp"

#include <algorithm>  // generate, count

#include <doctest/doctest.h>
#include <spdlog/spdlog.h>

#include "common/type/array.hpp"

using namespace tactile;

TEST_SUITE("RandomUtils")
{
  TEST_CASE("next_random_i32")
  {
    REQUIRE(next_random_i32(0, 0) == 0);
    REQUIRE(next_random_i32(1, 1) == 1);
    REQUIRE(next_random_i32(-1, -1) == -1);

    const auto value = next_random_u32(0u, 100u);
    REQUIRE(value >= 0u);
    REQUIRE(value <= 100u);
  }

  TEST_CASE("next_random_f32")
  {
    const auto value = next_random_f32(0.0f, 1.0f);
    REQUIRE(value >= 0.0f);
    REQUIRE(value <= 1.0f);
  }

  TEST_CASE("next_bool")
  {
    Array<bool, 1'000> values;
    std::generate(values.begin(), values.end(), next_bool);

    const auto true_count = std::count(values.begin(), values.end(), true);
    const auto false_count = std::ssize(values) - true_count;

    spdlog::debug("[Random] {} next_bool invocations: {} true, {} false",
                  values.size(),
                  true_count,
                  false_count);
  }

  TEST_CASE("next_normalized_float")
  {
    const auto value = next_normalized_float();
    REQUIRE(value >= 0.0f);
    REQUIRE(value <= 1.0f);
  }
}
