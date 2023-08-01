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

#include "common/util/vectors.hpp"

#include <doctest/doctest.h>

#include "core/containers/string.hpp"

using namespace tactile;

TEST_SUITE("VectorUtils")
{
  TEST_CASE("insert_at")
  {
    Vector<float> values = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    CHECK(values.size() == 5);

    // Out of bounds
    CHECK(insert_at(values, values.size() + 1, 6.0f).failed());
    CHECK(values.size() == 5);
    CHECK(values.at(4) == 5.0f);

    // Append
    CHECK(insert_at(values, values.size(), 6.0f).succeeded());
    CHECK(values.size() == 6);

    CHECK(values.at(4) == 5.0f);
    CHECK(values.at(5) == 6.0f);

    // Insert
    CHECK(insert_at(values, 2, 33.0f).succeeded());
    CHECK(values.size() == 7);

    CHECK(values.at(0) == 1.0f);
    CHECK(values.at(1) == 2.0f);
    CHECK(values.at(2) == 33.0f);
    CHECK(values.at(3) == 3.0f);
    CHECK(values.at(4) == 4.0f);
    CHECK(values.at(5) == 5.0f);
  }

  TEST_CASE("erase_at")
  {
    Vector<String> strings = {"A", "B", "C"};

    CHECK(erase_at(strings, 3).failed());
    CHECK(strings.size() == 3);
    CHECK(strings.at(0) == "A");
    CHECK(strings.at(1) == "B");
    CHECK(strings.at(2) == "C");

    CHECK(erase_at(strings, 1).succeeded());
    CHECK(strings.size() == 2);
    CHECK(strings.at(0) == "A");
    CHECK(strings.at(1) == "C");

    CHECK(erase_at(strings, 0).succeeded());
    CHECK(strings.size() == 1);
    CHECK(strings.at(0) == "C");

    CHECK(erase_at(strings, 0).succeeded());
    CHECK(strings.empty());

    CHECK(erase_at(strings, 0).failed());
  }

  TEST_CASE("contained_in")
  {
    const Vector<String> strings = {"A", "B", "C"};

    CHECK(contained_in(strings, "A"));
    CHECK(contained_in(strings, "B"));
    CHECK(contained_in(strings, "C"));
    CHECK(!contained_in(strings, "D"));
    CHECK(!contained_in(strings, "c"));
  }
}