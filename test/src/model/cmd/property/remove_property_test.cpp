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

#include "model/cmd/property/remove_property.hpp"

#include <doctest/doctest.h>

#include "core/debug/panic.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::RemoveProperty")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::RemoveProperty(nullptr, ""), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto map = map_document->get_map_ptr();
    auto& map_properties = map->get_ctx().props();

    const String property_name {"id"};
    const int property_value = 42;
    map_properties.add(property_name, property_value);

    cmd::RemoveProperty cmd {map, property_name};

    cmd.redo();
    REQUIRE(!map_properties.contains(property_name));
    REQUIRE(map_properties.empty());

    cmd.undo();
    REQUIRE(map_properties.contains(property_name));
    REQUIRE(property_value == map_properties.at(property_name).as_int());
  }
}

}  // namespace tactile::test
