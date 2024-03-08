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

#include "model/cmd/property/change_property_type.hpp"

#include <doctest/doctest.h>

#include "core/helpers/map_builder.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::test {

TEST_SUITE("cmd::ChangePropertyType")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::ChangePropertyType(nullptr, "", AttributeType::kInt),
                      Exception);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto map = map_document->get_map_ptr();
    auto& map_ctx = map->get_ctx();

    const String property_name {"foobar"};
    map_ctx.add_property(property_name, 123);

    cmd::ChangePropertyType cmd {map, property_name, AttributeType::kBool};

    cmd.redo();
    REQUIRE(map_ctx.has_property(property_name));
    REQUIRE(map_ctx.get_property(property_name).is_bool());
    REQUIRE(!map_ctx.get_property(property_name).as_bool());

    cmd.undo();
    REQUIRE(map_ctx.has_property(property_name));
    REQUIRE(map_ctx.get_property(property_name).is_int());
    REQUIRE(map_ctx.get_property(property_name).as_int() == 123);
  }
}

}  // namespace tactile::test
