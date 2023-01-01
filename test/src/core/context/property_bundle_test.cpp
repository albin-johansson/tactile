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

#include "core/context/property_bundle.hpp"

#include <doctest/doctest.h>

#include "core/debug/panic.hpp"

using namespace std::string_literals;

namespace tactile::test {

TEST_SUITE("PropertyBundle")
{
  TEST_CASE("Defaults")
  {
    const PropertyBundle bundle;
    REQUIRE(0u == bundle.size());
    REQUIRE(bundle.empty());
  }

  TEST_CASE("add[type]")
  {
    PropertyBundle bundle;

    bundle.add("foo", AttributeType::Int);
    REQUIRE(0 == bundle.at("foo").as_int());

    REQUIRE_THROWS_AS(bundle.add("foo", AttributeType::Float), TactileError);

    bundle.add("bar", AttributeType::Float);
    REQUIRE(0.0f == bundle.at("bar").as_float());

    REQUIRE(2u == bundle.size());
  }

  TEST_CASE("add[value]")
  {
    PropertyBundle bundle;

    bundle.add("foo", "bar"s);
    REQUIRE(bundle.contains("foo"));

    REQUIRE("bar" == bundle.at("foo").as_string());
    REQUIRE_THROWS_AS(bundle.add("foo", 42), TactileError);
  }

  TEST_CASE("update")
  {
    PropertyBundle bundle;
    REQUIRE_THROWS_AS(bundle.update("abc", 10), TactileError);

    bundle.add("qwerty", AttributeType::Int);
    REQUIRE(0 == bundle.at("qwerty"));

    bundle.update("qwerty", 42);
    REQUIRE(42 == bundle.at("qwerty"));

    bundle.update("qwerty", "str"s);
    REQUIRE("str"s == bundle.at("qwerty"));
  }

  TEST_CASE("remove")
  {
    PropertyBundle bundle;

    REQUIRE(!bundle.remove("foo"));
    bundle.add("foo", 123);

    REQUIRE(!bundle.remove("bar"));
    REQUIRE(!bundle.empty());

    REQUIRE(bundle.remove("foo"));
    REQUIRE(bundle.empty());
  }

  TEST_CASE("rename")
  {
    PropertyBundle bundle;

    REQUIRE(!bundle.rename("A", "B"));
    bundle.add("A", Color {0, 0xFF, 0xFF});

    // Cannot request used name, even when the target property doesn't exist
    REQUIRE_THROWS_AS(bundle.rename("C", "A"), TactileError);

    REQUIRE(!bundle.rename("C", "B"));
    REQUIRE(bundle.contains("A"));
    REQUIRE(!bundle.contains("B"));

    REQUIRE(bundle.rename("A", "B"));
    REQUIRE(!bundle.contains("A"));
    REQUIRE(bundle.contains("B"));

    REQUIRE(1u == bundle.size());
  }

  TEST_CASE("change_type")
  {
    PropertyBundle bundle;
    bundle.add("abc", AttributeType::Int);
    REQUIRE(0 == bundle.at("abc").as_int());

    REQUIRE(0 == bundle.change_type("abc", AttributeType::Bool));
    REQUIRE(!bundle.at("abc").as_bool());
  }

  TEST_CASE("find")
  {
    PropertyBundle bundle;
    const auto& ref = bundle;

    REQUIRE(!bundle.find("foo"));
    REQUIRE(!ref.find("foo"));

    bundle.add("foo", 123);

    REQUIRE(123 == bundle.find("foo")->as_int());
    REQUIRE(123 == ref.find("foo")->as_int());
  }

  TEST_CASE("at")
  {
    PropertyBundle bundle;
    const auto& ref = bundle;

    REQUIRE_THROWS_AS(bundle.at("foo"), TactileError);
    REQUIRE_THROWS_AS(ref.at("foo"), TactileError);

    bundle.add("foo", true);

    REQUIRE(bundle.at("foo").as_bool());
    REQUIRE(ref.at("foo").as_bool());
  }

  TEST_CASE("contains")
  {
    PropertyBundle bundle;
    REQUIRE(!bundle.contains("xyz"));

    bundle.add("xyz", 4.5f);
    REQUIRE(bundle.contains("xyz"));
    REQUIRE(!bundle.contains("XYZ"));
  }
}

}  // namespace tactile::test
