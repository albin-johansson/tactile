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

#include "common/util/numeric.hpp"

#include <doctest/doctest.h>

using namespace tactile;

TEST_SUITE("NumericUtilities")
{
  TEST_CASE("compare_vector_components")
  {
    SUBCASE("All components match")
    {
      const Int4 a {1, 2, 3, 4};
      const Int4 b {1, 2, 3, 4};
      const auto mask = compare_vector_components(a, b);
      CHECK((mask & kVectorXBit));
      CHECK((mask & kVectorYBit));
      CHECK((mask & kVectorZBit));
      CHECK((mask & kVectorWBit));
    }

    SUBCASE("X component differs")
    {
      const Int4 a {9, 2, 3, 4};
      const Int4 b {1, 2, 3, 4};
      const auto mask = compare_vector_components(a, b);
      CHECK(!(mask & kVectorXBit));
      CHECK((mask & kVectorYBit));
      CHECK((mask & kVectorZBit));
      CHECK((mask & kVectorWBit));
    }

    SUBCASE("Y component differs")
    {
      const Int4 a {1, 2, 3, 4};
      const Int4 b {1, 9, 3, 4};
      const auto mask = compare_vector_components(a, b);
      CHECK((mask & kVectorXBit));
      CHECK(!(mask & kVectorYBit));
      CHECK((mask & kVectorZBit));
      CHECK((mask & kVectorWBit));
    }

    SUBCASE("Z component differs")
    {
      const Int4 a {1, 2, 9, 4};
      const Int4 b {1, 2, 3, 4};
      const auto mask = compare_vector_components(a, b);
      CHECK((mask & kVectorXBit));
      CHECK((mask & kVectorYBit));
      CHECK(!(mask & kVectorZBit));
      CHECK((mask & kVectorWBit));
    }

    SUBCASE("W component differs")
    {
      const Int4 a {1, 2, 3, 4};
      const Int4 b {1, 2, 3, 9};
      const auto mask = compare_vector_components(a, b);
      CHECK((mask & kVectorXBit));
      CHECK((mask & kVectorYBit));
      CHECK((mask & kVectorZBit));
      CHECK(!(mask & kVectorWBit));
    }
  }
}
