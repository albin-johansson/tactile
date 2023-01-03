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

#include "common/util/assoc.hpp"

#include <doctest/doctest.h>

#include "common/type/hash_map.hpp"
#include "common/type/string.hpp"
#include "common/type/string_map.hpp"
#include "common/type/tree_map.hpp"
#include "core/attribute.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace tactile::test {

TEST_SUITE("Assoc")
{
  TEST_CASE("Empty HashMap")
  {
    const StringMap<int> map;

    REQUIRE(!has_key(map, "foo"));
    REQUIRE_THROWS_AS(lookup_in(map, "foo"), TactileError);
  }

  TEST_CASE("Empty TreeMap")
  {
    const TreeMap<String, int> map;

    REQUIRE(!has_key(map, "foo"));
    REQUIRE_THROWS_AS(lookup_in(map, "foo"), TactileError);
  }

  TEST_CASE("lookup_in[HashMap]")
  {
    StringMap<Attribute> map;
    map["foo"] = 3.5f;
    map["bar"] = true;

    REQUIRE_THROWS_AS(lookup_in(map, ""), TactileError);
    REQUIRE_THROWS_AS(lookup_in(map, "foO"), TactileError);
    REQUIRE_THROWS_AS(lookup_in(map, "BAR"), TactileError);

    REQUIRE(3.5f == lookup_in(map, "foo"));
    REQUIRE(true == lookup_in(map, "bar"));
  }

  TEST_CASE("lookup_in[TreeMap]")
  {
    TreeMap<int, Attribute> map;
    map[10] = "hello"s;
    map[11] = Color {0xFF, 0, 0};

    REQUIRE_THROWS_AS(lookup_in(map, 9), TactileError);
    REQUIRE_THROWS_AS(lookup_in(map, 12), TactileError);

    REQUIRE("hello"s == lookup_in(map, 10));
    REQUIRE(Color {0xFF, 0, 0} == lookup_in(map, 11));
  }

  TEST_CASE("has_key[HashMap]")
  {
    StringMap<int> map;
    map["foo"] = 10;
    map["bar"] = 20;
    map["abc"] = 30;

    REQUIRE(3u == map.size());

    REQUIRE(has_key(map, "foo"));
    REQUIRE(has_key(map, "bar"));
    REQUIRE(has_key(map, "abc"));

    REQUIRE(!has_key(map, ""));
    REQUIRE(!has_key(map, "fooo"));
    REQUIRE(!has_key(map, "foO"));
    REQUIRE(!has_key(map, "BAR"));
    REQUIRE(!has_key(map, "bar "));
  }

  TEST_CASE("has_key[TreeMap]")
  {
    TreeMap<StringView, float> map;
    map["A"] = 1.0f;
    map["B"] = 42.0f;

    REQUIRE(2u == map.size());

    REQUIRE(has_key(map, "A"));
    REQUIRE(has_key(map, "A"s));
    REQUIRE(has_key(map, "A"sv));
    REQUIRE(has_key(map, "B"));
    REQUIRE(has_key(map, "B"s));
    REQUIRE(has_key(map, "B"sv));

    REQUIRE(!has_key(map, ""));
    REQUIRE(!has_key(map, "a"));
    REQUIRE(!has_key(map, "b"));
    REQUIRE(!has_key(map, "c"));
    REQUIRE(!has_key(map, "c"s));
    REQUIRE(!has_key(map, "c"sv));
  }
}

}  // namespace tactile::test