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

#include "core/prelude.hpp"

using namespace tactile;

namespace {

constexpr auto kMaxU32 = std::numeric_limits<uint32>::max();

}  // namespace

TEST_SUITE("AlgorithmUtils")
{
  TEST_CASE("udiff")
  {
    REQUIRE(udiff(kMaxU32, kMaxU32 - 1) == 1u);
    REQUIRE(udiff(kMaxU32 - 1, kMaxU32) == 1u);
    REQUIRE(udiff(0u, kMaxU32) == kMaxU32);
    REQUIRE(udiff(28u, 74u) == 46u);
  }
}
