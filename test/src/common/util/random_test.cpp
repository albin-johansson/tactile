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

#include "tactile/core/numeric/random.hpp"

#include <algorithm>  // generate, count

#include <doctest/doctest.h>
#include <spdlog/spdlog.h>

#include "tactile/base/container/array.hpp"

namespace tactile::test {

TEST_SUITE("Random")
{
  TEST_CASE("get_random_int")
  {
    REQUIRE(0 == get_random_int(0, 0));
    REQUIRE(1 == get_random_int(1, 1));
    REQUIRE(-1 == get_random_int(-1, -1));

    const auto value = get_random_int(53, 832);
    REQUIRE(value >= 53);
    REQUIRE(value <= 832);
  }

  TEST_CASE("get_random_uint")
  {
    const auto value = get_random_uint(0u, 100u);
    REQUIRE(value >= 0u);
    REQUIRE(value <= 100u);
  }

  TEST_CASE("get_random_float")
  {
    const auto value = get_random_float(0.0f, 1.0f);
    REQUIRE(value >= 0.0f);
    REQUIRE(value <= 1.0f);
  }

  TEST_CASE("get_random_bool")
  {
    Array<bool, 1'000> values;
    std::generate(values.begin(), values.end(), &get_random_bool);

    const auto true_count = std::count(values.begin(), values.end(), true);
    const auto false_count = std::ssize(values) - true_count;

    spdlog::debug("[Random] {} get_random_bool invocations: {} true, {} false",
                  values.size(),
                  true_count,
                  false_count);
  }

  TEST_CASE("get_random_float_normalized")
  {
    const auto value = get_random_float_normalized();
    REQUIRE(value >= 0.0f);
    REQUIRE(value <= 1.0f);
  }
}

}  // namespace tactile::test
