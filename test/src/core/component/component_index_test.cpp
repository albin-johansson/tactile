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

#include "core/component/component_definition.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::test {

TEST_SUITE("ComponentIndex")
{
  TEST_CASE("Defaults")
  {
    const ComponentIndex index;
    REQUIRE(index.comp_count() == 0u);
    REQUIRE(index.empty());
  }

  TEST_CASE("define")
  {
    ComponentIndex index;
    const auto component_id = index.define_comp("position");

    const auto& component_def = index.get_comp(component_id);
    REQUIRE(component_def.get_name() == "position");
    REQUIRE(component_def.get_uuid() == component_id);
    REQUIRE(component_def.empty());

    REQUIRE(index.has_comp("position"));
    REQUIRE(index.comp_count() == 1u);

    REQUIRE_THROWS_AS(index.define_comp("position"), Exception);
  }

  TEST_CASE("remove")
  {
    ComponentIndex index;
    REQUIRE(!index.remove_comp(UUID::generate()).has_value());

    const auto component_id = index.define_comp("foo");
    REQUIRE(index.comp_count() == 1u);
    REQUIRE(index.has_comp("foo"));

    REQUIRE(index.remove_comp(component_id).has_value());

    REQUIRE(0u == index.comp_count());
    REQUIRE(!index.has_comp("foo"));
  }

  TEST_CASE("rename")
  {
    ComponentIndex index;
    REQUIRE(!index.rename_comp(UUID::generate(), "foo").has_value());

    const auto component_id = index.define_comp("foo");

    REQUIRE(index.has_comp("foo"));
    REQUIRE(!index.has_comp("zoo"));

    index.rename_comp(component_id, "zoo");

    REQUIRE(!index.has_comp("foo"));
    REQUIRE(index.has_comp("zoo"));

    index.define_comp("woo");
    REQUIRE(!index.rename_comp(component_id, "woo").has_value());
  }
}

}  // namespace tactile::test
