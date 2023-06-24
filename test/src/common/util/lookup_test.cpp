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

#include "common/util/lookup.hpp"

#include <doctest/doctest.h>

#include "common/attribute.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/string.hpp"
#include "common/type/string_map.hpp"
#include "common/type/tree_map.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

using namespace tactile;

TEST_SUITE("LookupUtils")
{
  TEST_CASE("Empty HashMap")
  {
    const StringMap<int> map;

    CHECK(!has_key(map, "foo"));
    CHECK_THROWS_AS(lookup_in(map, "foo"), TactileError);
  }

  TEST_CASE("Empty TreeMap")
  {
    const TreeMap<String, int> map;

    CHECK(!has_key(map, "foo"));
    CHECK_THROWS_AS(lookup_in(map, "foo"), TactileError);
  }

  TEST_CASE("lookup_in[HashMap]")
  {
    StringMap<Attribute> map;
    map["foo"] = 3.5f;
    map["bar"] = true;

    CHECK_THROWS_AS(lookup_in(map, ""), TactileError);
    CHECK_THROWS_AS(lookup_in(map, "foO"), TactileError);
    CHECK_THROWS_AS(lookup_in(map, "BAR"), TactileError);

    CHECK(lookup_in(map, "foo") == 3.5f);
    CHECK(lookup_in(map, "bar") == true);
  }

  TEST_CASE("lookup_in[TreeMap]")
  {
    TreeMap<int, Attribute> map;
    map[10] = "hello"s;
    map[11] = Color {0xFF, 0, 0};

    CHECK_THROWS_AS(lookup_in(map, 9), TactileError);
    CHECK_THROWS_AS(lookup_in(map, 12), TactileError);

    CHECK(lookup_in(map, 10) == "hello"s);
    CHECK(lookup_in(map, 11) == Color {0xFF, 0, 0});
  }

  TEST_CASE("has_key[HashMap]")
  {
    StringMap<int> map;
    map["foo"] = 10;
    map["bar"] = 20;
    map["abc"] = 30;

    CHECK(map.size() == 3u);

    CHECK(has_key(map, "foo"));
    CHECK(has_key(map, "bar"));
    CHECK(has_key(map, "abc"));

    CHECK(!has_key(map, ""));
    CHECK(!has_key(map, "fooo"));
    CHECK(!has_key(map, "foO"));
    CHECK(!has_key(map, "BAR"));
    CHECK(!has_key(map, "bar "));
  }

  TEST_CASE("has_key[TreeMap]")
  {
    TreeMap<StringView, float> map;
    map["A"] = 1.0f;
    map["B"] = 42.0f;

    CHECK(map.size() == 2u);

    CHECK(has_key(map, "A"));
    CHECK(has_key(map, "A"s));
    CHECK(has_key(map, "A"sv));
    CHECK(has_key(map, "B"));
    CHECK(has_key(map, "B"s));
    CHECK(has_key(map, "B"sv));

    CHECK(!has_key(map, ""));
    CHECK(!has_key(map, "a"));
    CHECK(!has_key(map, "b"));
    CHECK(!has_key(map, "c"));
    CHECK(!has_key(map, "c"s));
    CHECK(!has_key(map, "c"sv));
  }
}
