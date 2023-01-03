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

#include "core/component/component_base.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"

using namespace std::string_literals;

namespace tactile::test {

TEST_SUITE("ComponentBase")
{
  TEST_CASE("Defaults")
  {
    const ComponentBase component {make_uuid()};
    REQUIRE(component.empty());
    REQUIRE(0u == component.size());

    usize count = 0;
    for (const auto& x [[maybe_unused]]: component) {
      ++count;
    }

    REQUIRE(0u == count);
  }

  TEST_CASE("add[type]")
  {
    ComponentBase component {make_uuid()};

    component.add("str");
    REQUIRE(""s == component.at("str"));

    component.add("int", AttributeType::Int);
    REQUIRE(0 == component.at("int"));

    component.add("bool", AttributeType::Bool);
    REQUIRE(false == component.at("bool"));
  }

  TEST_CASE("add[value]")
  {
    ComponentBase component {make_uuid()};

    component.add("A", 10);
    REQUIRE(10 == component.at("A"));

    REQUIRE(component.has("A"));
    REQUIRE(!component.has("a"));

    REQUIRE(1u == component.size());

    REQUIRE_THROWS_AS(component.add("A"), TactileError);
  }

  TEST_CASE("update")
  {
    ComponentBase component {make_uuid()};
    REQUIRE_THROWS_AS(component.update("foo", 10), TactileError);

    component.add("foo");
    REQUIRE(""s == component.at("foo"));
    REQUIRE(1u == component.size());

    component.update("foo", "bar"s);
    REQUIRE("bar"s == component.at("foo"));
    REQUIRE(1u == component.size());
  }

  TEST_CASE("remove")
  {
    ComponentBase component {make_uuid()};
    REQUIRE(!component.remove("foo"));

    component.add("foo", Color {0xFF, 0, 0});
    REQUIRE(Color {0xFF, 0, 0} == component.at("foo"));
    REQUIRE(!component.empty());

    REQUIRE(component.remove("foo"));
    REQUIRE(component.empty());
  }

  TEST_CASE("rename")
  {
    ComponentBase component {make_uuid()};
    REQUIRE(!component.rename("foo", "bar"));

    component.add("foo", 123);
    REQUIRE(component.rename("foo", "bar"));
    REQUIRE(!component.rename("abc", "def"));

    REQUIRE_THROWS_AS(component.rename("", "bar"), TactileError);
  }

  TEST_CASE("duplicate")
  {
    ComponentBase component {make_uuid()};
    REQUIRE_THROWS_AS(component.duplicate("abc"), TactileError);

    component.add("abc", 3.5f);
    REQUIRE(component.has("abc"));
    REQUIRE(1u == component.size());

    const auto new_name = component.duplicate("abc");
    REQUIRE(component.has("abc"));
    REQUIRE(component.has(new_name));
    REQUIRE(3.5f == component.at(new_name));
    REQUIRE(2u == component.size());
  }
}

}  // namespace tactile::test