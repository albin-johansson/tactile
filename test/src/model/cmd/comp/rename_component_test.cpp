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

#include "model/cmd/comp/rename_component.hpp"

#include <doctest/doctest.h>

#include "core/debug/panic.hpp"
#include "core/helpers/component_builder.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::RenameComponent")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::RenameComponent(nullptr, make_uuid(), ""), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto component_index = map_document->get_component_index_ptr();

    const String old_component_name {"A"};
    const String new_component_name {"B"};

    const auto component_id =
        ComponentBuilder {component_index, old_component_name}.result();

    auto& map = map_document->get_map();
    auto& component_bundle = map.get_ctx().comps();
    component_bundle.add(component_index->at(component_id).instantiate());

    cmd::RenameComponent cmd {component_index, component_id, new_component_name};

    cmd.redo();
    REQUIRE(component_index->contains(component_id));
    REQUIRE(!component_index->contains(old_component_name));
    REQUIRE(component_index->contains(new_component_name));
    REQUIRE(component_bundle.size() == 1u);
    REQUIRE(component_bundle.contains(component_id));

    cmd.undo();
    REQUIRE(component_index->contains(component_id));
    REQUIRE(component_index->contains(old_component_name));
    REQUIRE(!component_index->contains(new_component_name));
    REQUIRE(component_bundle.size() == 1u);
    REQUIRE(component_bundle.contains(component_id));
  }
}

}  // namespace tactile::test
