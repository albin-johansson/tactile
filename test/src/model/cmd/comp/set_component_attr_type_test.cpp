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

#include "model/cmd/comp/set_component_attr_type.hpp"

#include <doctest/doctest.h>

#include "core/helpers/component_builder.hpp"
#include "core/helpers/map_builder.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::test {

TEST_SUITE("cmd::SetComponentAttrType")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(
        cmd::SetComponentAttrType(nullptr, UUID::generate(), "", AttributeType::kInt),
        Exception);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto component_index = map_document->get_component_index_ptr();

    const String attr_name {"Attr1"};
    const bool old_attr_value = true;

    const auto old_attr_type = AttributeType::kBool;
    const auto new_attr_type = AttributeType::kInt;

    const auto component_id = ComponentBuilder {component_index, "Demo"}  //
                                  .with_attr(attr_name, old_attr_value)
                                  .with_attr("Attr2", 938)
                                  .with_attr("Attr3", 94.3f)
                                  .result();

    auto& map = map_document->get_map();
    auto& map_ctx = map.get_ctx();
    map_ctx.attach_component(component_index->get_comp(component_id).instantiate());

    cmd::SetComponentAttrType cmd {map_document.get(),
                                   component_id,
                                   attr_name,
                                   new_attr_type};

    {
      cmd.redo();

      const auto& component_def = component_index->get_comp(component_id);
      const auto& component = map_ctx.get_component(component_id);

      REQUIRE(component_def.get_attr(attr_name).get_type() == new_attr_type);
      REQUIRE(component.get_attr(attr_name).get_type() == new_attr_type);

      REQUIRE(component_def.get_attr(attr_name).as_int() == Attribute {new_attr_type});
      REQUIRE(component.get_attr(attr_name).as_int() == Attribute {new_attr_type});
    }

    {
      cmd.undo();

      const auto& component_def = component_index->get_comp(component_id);
      const auto& component = map_ctx.get_component(component_id);

      REQUIRE(component_def.get_attr(attr_name).get_type() == old_attr_type);
      REQUIRE(component.get_attr(attr_name).get_type() == old_attr_type);

      REQUIRE(component_def.get_attr(attr_name) == old_attr_value);
      REQUIRE(component.get_attr(attr_name) == old_attr_value);
    }
  }
}

}  // namespace tactile::test
