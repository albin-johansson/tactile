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

#include "common/util/algorithms.hpp"

#include <limits>  // numeric_limits

#include <doctest/doctest.h>

#include "common/primitives.hpp"

using namespace tactile;

namespace {

constexpr auto kMaxU32 = std::numeric_limits<uint32>::max();

}  // namespace

TEST_SUITE("Algorithm utilities")
{
  TEST_CASE("udiff")
  {
    REQUIRE(1u == udiff(kMaxU32, kMaxU32 - 1));
    REQUIRE(1u == udiff(kMaxU32 - 1, kMaxU32));

    REQUIRE(kMaxU32 == udiff(0u, kMaxU32));

    REQUIRE(46u == udiff(28u, 74u));
  }
}
