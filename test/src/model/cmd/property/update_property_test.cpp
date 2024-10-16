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

#include "model/cmd/property/update_property.hpp"

#include <doctest/doctest.h>

#include "core/helpers/map_builder.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::test {

TEST_SUITE("cmd::UpdateProperty")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::UpdateProperty(nullptr, "", 0), Exception);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto map = map_document->get_map_ptr();
    auto& map_ctx = map->get_ctx();

    const String property_name {"wow"};
    const int old_property_value = 10;
    const int new_property_value = 20;
    map_ctx.add_property(property_name, old_property_value);

    cmd::UpdateProperty cmd {map, property_name, new_property_value};

    cmd.redo();
    REQUIRE(map_ctx.get_property(property_name) == new_property_value);

    cmd.undo();
    REQUIRE(map_ctx.get_property(property_name) == old_property_value);
  }

  TEST_CASE("merge_with")
  {
    auto map_document = MapBuilder::build().result();
    auto map = map_document->get_map_ptr();

    const String property_name {"cool-color"};
    const float initial_property_value = 10;

    auto& map_ctx = map->get_ctx();
    map_ctx.add_property(property_name, initial_property_value);

    cmd::UpdateProperty a {map, property_name, initial_property_value + 1.0f};
    const cmd::UpdateProperty b {map, property_name, initial_property_value + 2.0f};
    const cmd::UpdateProperty c {map, property_name, initial_property_value + 3.0f};

    REQUIRE(a.merge_with(&b));
    REQUIRE(a.merge_with(&c));

    a.redo();
    REQUIRE(map_ctx.get_property(property_name) == initial_property_value + 3.0f);

    a.undo();
    REQUIRE(map_ctx.get_property(property_name) == initial_property_value);
  }
}

}  // namespace tactile::test
