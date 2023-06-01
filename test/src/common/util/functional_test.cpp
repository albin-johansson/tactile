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

#include "common/util/functional.hpp"

#include <doctest/doctest.h>

using namespace tactile;

TEST_SUITE("Functional")
{
  TEST_CASE("invoke_n")
  {
    SUBCASE("zero invocations")
    {
      bool called = false;
      invoke_n(0, [&] { called = true; });
      REQUIRE(!called);
    }

    SUBCASE("one invocation")
    {
      int calls = 0;
      invoke_n(1, [&] { ++calls; });
      REQUIRE(1 == calls);
    }

    SUBCASE("several invocations")
    {
      int calls = 0;
      invoke_n(42, [&] { ++calls; });
      REQUIRE(42 == calls);
    }
  }
}
