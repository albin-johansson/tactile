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

#include "core/component/component_index.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/component/component_definition.hpp"

namespace tactile::test {

TEST_SUITE("ComponentIndex")
{
  TEST_CASE("Defaults")
  {
    const ComponentIndex index;
    REQUIRE(0u == index.size());
    REQUIRE(index.empty());
  }

  TEST_CASE("define")
  {
    ComponentIndex index;
    const auto component_id = index.define("position");

    const auto& component_def = index.at(component_id);
    REQUIRE("position" == component_def.get_name());
    REQUIRE(component_id == component_def.get_uuid());
    REQUIRE(component_def.empty());

    REQUIRE(index.contains("position"));
    REQUIRE(1u == index.size());

    REQUIRE_THROWS_AS(index.define("position"), TactileError);
  }

  TEST_CASE("remove")
  {
    ComponentIndex index;
    REQUIRE_THROWS_AS(index.remove(make_uuid()), TactileError);

    const auto component_id = index.define("foo");
    REQUIRE(1u == index.size());
    REQUIRE(index.contains("foo"));

    index.remove(component_id);

    REQUIRE(0u == index.size());
    REQUIRE(!index.contains("foo"));
  }

  TEST_CASE("rename")
  {
    ComponentIndex index;
    REQUIRE_THROWS_AS(index.rename(make_uuid(), "foo"), TactileError);

    const auto id = index.define("foo");

    REQUIRE(index.contains("foo"));
    REQUIRE(!index.contains("zoo"));

    index.rename(id, "zoo");

    REQUIRE(!index.contains("foo"));
    REQUIRE(index.contains("zoo"));

    index.define("woo");
    REQUIRE_THROWS_AS(index.rename(id, "woo"), TactileError);
  }
}

}  // namespace tactile::test
