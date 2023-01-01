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

#include "model/cmd/comp/add_component_attr.hpp"

#include <doctest/doctest.h>

#include "core/debug/panic.hpp"
#include "core/helpers/component_builder.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::AddComponentAttr")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::AddComponentAttr(nullptr, make_uuid(), ""), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto component_index = map_document->get_component_index_ptr();

    const auto component_id = ComponentBuilder {component_index, "Demo"}.result();
    auto& definition = component_index->at(component_id);
    REQUIRE(definition.empty());

    auto& map = map_document->get_map();
    auto& component_bundle = map.get_ctx().comps();

    component_bundle.add(definition.instantiate());
    REQUIRE(component_bundle.contains(component_id));
    REQUIRE(component_bundle.at(component_id).empty());

    const String attr_name {"foo"};
    cmd::AddComponentAttr cmd {map_document.get(), component_id, attr_name};

    cmd.redo();
    REQUIRE(definition.size() == 1u);
    REQUIRE(definition.has(attr_name));
    REQUIRE(definition.at(attr_name) == component_bundle.at(component_id).at(attr_name));

    cmd.undo();
    REQUIRE(definition.empty());
    REQUIRE(!definition.has(attr_name));
    REQUIRE_THROWS_AS(component_bundle.at(component_id).at(attr_name), TactileError);
  }
}

}  // namespace tactile::test
