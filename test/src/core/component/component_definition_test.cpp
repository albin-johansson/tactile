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

#include "core/component/component_definition.hpp"

#include <doctest/doctest.h>

using namespace std::string_literals;

namespace tactile::test {

TEST_SUITE("ComponentDefinition")
{
  TEST_CASE("set_name")
  {
    ComponentDefinition definition;
    REQUIRE(definition.name().empty());

    definition.set_name("Qwerty");
    REQUIRE("Qwerty" == definition.name());
  }

  TEST_CASE("instantiate")
  {
    ComponentDefinition definition;
    definition.add_attr("i", 42);
    definition.add_attr("f", 1.8f);
    definition.add_attr("s", "foo"s);
    definition.add_attr("c", Color {0xFF, 0xD7, 0});

    const auto instance = definition.instantiate();
    REQUIRE(definition.get_uuid() == instance.definition_id());
    REQUIRE(definition.attr_count() == instance.attr_count());

    for (const auto& [key, value]: instance) {
      REQUIRE(definition.get_attr(key) == value);
    }
  }
}

}  // namespace tactile::test
