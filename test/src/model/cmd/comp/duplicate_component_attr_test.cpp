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

#include "model/cmd/comp/duplicate_component_attr.hpp"

#include <doctest/doctest.h>

#include "core/debug/panic.hpp"
#include "core/helpers/component_builder.hpp"
#include "core/helpers/map_builder.hpp"

namespace tactile::test {

TEST_SUITE("cmd::DuplicateComponentAttr")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::DuplicateComponentAttr(nullptr, make_uuid(), ""),
                      TactileError);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto component_index = map_document->get_component_index_ptr();

    const auto component_id = ComponentBuilder {component_index, "Demo"}  //
                                  .with_attr("Attr", 42)
                                  .result();

    auto& map = map_document->get_map();
    auto& map_ctx = map.get_ctx();
    map_ctx.attach_component(component_index->at(component_id).instantiate());

    const String attr_name {"Attr"};
    const String duplicated_attr_name {"Attr (1)"};

    cmd::DuplicateComponentAttr cmd {map_document.get(), component_id, attr_name};
    cmd.redo();

    {
      const auto& component_def = component_index->at(component_id);
      const auto& component = map_ctx.get_component(component_id);

      REQUIRE(component_def.size() == 2u);
      REQUIRE(component_def.has(attr_name));
      REQUIRE(component_def.has(duplicated_attr_name));
      REQUIRE(component_def.at(attr_name) == component_def.at(duplicated_attr_name));

      REQUIRE(component.size() == 2u);
      REQUIRE(component.has(attr_name));
      REQUIRE(component.has(duplicated_attr_name));
      REQUIRE(component.at(attr_name) == component.at(duplicated_attr_name));
    }

    cmd.undo();

    {
      const auto& component_def = component_index->at(component_id);
      const auto& component = map_ctx.get_component(component_id);

      REQUIRE(component_def.size() == 1u);
      REQUIRE(component_def.has(attr_name));
      REQUIRE(!component_def.has(duplicated_attr_name));

      REQUIRE(component.size() == 1u);
      REQUIRE(component.has(attr_name));
      REQUIRE(!component.has(duplicated_attr_name));
    }
  }
}

}  // namespace tactile::test
