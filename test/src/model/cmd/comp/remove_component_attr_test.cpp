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

#include "model/cmd/comp/remove_component_attr.hpp"

#include <doctest/doctest.h>

#include "core/debug/panic.hpp"
#include "core/helpers/component_builder.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::RemoveComponentAttr")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::RemoveComponentAttr(nullptr, make_uuid(), ""), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto component_index = map_document->get_component_index_ptr();

    const String attr_name {"test"};
    const int attr_value = 42;

    const auto component_id = ComponentBuilder {component_index, "Demo"}
                                  .with_attr(attr_name, attr_value)
                                  .result();

    auto& component_def = component_index->at(component_id);
    auto& map = map_document->get_map();
    auto& component_bundle = map.get_ctx().comps();

    component_bundle.add(component_def.instantiate());
    REQUIRE(component_bundle.contains(component_id));
    REQUIRE(component_bundle.at(component_id).at(attr_name) == attr_value);

    cmd::RemoveComponentAttr cmd {map_document.get(), component_id, attr_name};

    cmd.redo();
    REQUIRE(component_def.empty());
    REQUIRE(component_bundle.at(component_id).empty());

    cmd.undo();
    REQUIRE(component_def.has(attr_name));
    REQUIRE(component_def.at(attr_name) == attr_value);

    REQUIRE(component_bundle.at(component_id).has(attr_name));
    REQUIRE(component_def.at(attr_name) ==
            component_bundle.at(component_id).at(attr_name));
  }
}

}  // namespace tactile::test
