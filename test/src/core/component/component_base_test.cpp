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
    REQUIRE(0u == component.attr_count());

    usize count = 0;
    for (const auto& x [[maybe_unused]]: component) {
      ++count;
    }

    REQUIRE(0u == count);
  }

  TEST_CASE("add_attr[type]")
  {
    ComponentBase component {make_uuid()};

    component.add_attr("str");
    REQUIRE(""s == component.get_attr("str"));

    component.add_attr("int", AttributeType::Int);
    REQUIRE(0 == component.get_attr("int"));

    component.add_attr("bool", AttributeType::Bool);
    REQUIRE(false == component.get_attr("bool"));
  }

  TEST_CASE("add_attr[value]")
  {
    ComponentBase component {make_uuid()};

    component.add_attr("A", 10);
    REQUIRE(10 == component.get_attr("A"));

    REQUIRE(component.has_attr("A"));
    REQUIRE(!component.has_attr("a"));

    REQUIRE(1u == component.attr_count());

    REQUIRE_THROWS_AS(component.add_attr("A"), TactileError);
  }

  TEST_CASE("update_attr")
  {
    ComponentBase component {make_uuid()};
    REQUIRE_THROWS_AS(component.update_attr("foo", 10), TactileError);

    component.add_attr("foo");
    REQUIRE(""s == component.get_attr("foo"));
    REQUIRE(1u == component.attr_count());

    component.update_attr("foo", "bar"s);
    REQUIRE("bar"s == component.get_attr("foo"));
    REQUIRE(1u == component.attr_count());
  }

  TEST_CASE("remove_attr")
  {
    ComponentBase component {make_uuid()};
    REQUIRE(component.remove_attr("foo").failed());

    component.add_attr("foo", Color {0xFF, 0, 0});
    REQUIRE(Color {0xFF, 0, 0} == component.get_attr("foo"));
    REQUIRE(!component.empty());

    REQUIRE(component.remove_attr("foo").succeeded());
    REQUIRE(component.empty());
  }

  TEST_CASE("rename_attr")
  {
    ComponentBase component {make_uuid()};
    REQUIRE(component.rename_attr("foo", "bar").failed());

    component.add_attr("foo", 123);
    REQUIRE(component.rename_attr("foo", "bar").succeeded());
    REQUIRE(component.rename_attr("abc", "def").failed());

    REQUIRE(component.rename_attr("", "bar").failed());
  }

  TEST_CASE("duplicate_attr")
  {
    ComponentBase component {make_uuid()};
    REQUIRE_THROWS_AS(component.duplicate_attr("abc"), TactileError);

    component.add_attr("abc", 3.5f);
    REQUIRE(component.has_attr("abc"));
    REQUIRE(1u == component.attr_count());

    const auto new_name = component.duplicate_attr("abc");
    REQUIRE(component.has_attr("abc"));
    REQUIRE(component.has_attr(new_name));
    REQUIRE(3.5f == component.get_attr(new_name));
    REQUIRE(2u == component.attr_count());
  }
}

}  // namespace tactile::test