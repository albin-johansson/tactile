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

#include "core/helpers/map_builder.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::test {

TEST_SUITE("cmd::RemoveProperty")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::RemoveProperty(nullptr, ""), Exception);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto map = map_document->get_map_ptr();
    auto& map_ctx = map->get_ctx();

    const String property_name {"id"};
    const int property_value = 42;
    map_ctx.add_property(property_name, property_value);

    cmd::RemoveProperty cmd {map, property_name};

    cmd.redo();
    REQUIRE(!map_ctx.has_property(property_name));
    REQUIRE(map_ctx.property_count() == 0u);

    cmd.undo();
    REQUIRE(map_ctx.has_property(property_name));
    REQUIRE(map_ctx.get_property(property_name).as_int() == property_value);
  }
}

}  // namespace tactile::test
