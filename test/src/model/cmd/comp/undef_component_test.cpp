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

#include "model/cmd/comp/undef_component.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/helpers/component_builder.hpp"
#include "core/helpers/map_builder.hpp"

using namespace std::string_literals;

namespace tactile::test {

TEST_SUITE("cmd::UndefComponent")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::UndefComponent(nullptr, make_uuid()), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    auto map_document = MapBuilder::build().result();
    auto component_index = map_document->get_component_index_ptr();

    const String component_name {"Demo"};

    const String attr1_name {"foo"};
    const String attr1_value {"bar"};

    const String attr2_name {"threshold"};
    const float attr2_value = 1.5f;

    const auto component_id = ComponentBuilder {component_index, component_name}
                                  .with_attr(attr1_name, attr1_value)
                                  .with_attr(attr2_name, attr2_value)
                                  .result();

    auto& map = map_document->get_map();
    auto& map_ctx = map.get_ctx();

    const String new_attr1_value {"abc"};
    const float new_attr2_value = 8.9f;

    // Update the attributes in the attached component
    map_ctx.attach_component(component_index->get_comp(component_id).instantiate());
    map_ctx.get_component(component_id).update_attr(attr1_name, new_attr1_value);
    map_ctx.get_component(component_id).update_attr(attr2_name, new_attr2_value);

    cmd::UndefComponent cmd {map_document.get(), component_id};

    cmd.redo();
    REQUIRE(!component_index->has_comp(component_name));
    REQUIRE(!component_index->has_comp(component_id));
    REQUIRE(!map_ctx.has_component(component_id));
    REQUIRE(map_ctx.component_count() == 0u);

    cmd.undo();
    REQUIRE(component_index->has_comp(component_name));
    REQUIRE(component_index->has_comp(component_id));

    REQUIRE(component_index->get_comp(component_id).get_attr(attr1_name).as_string() ==
            attr1_value);
    REQUIRE(component_index->get_comp(component_id).get_attr(attr2_name).as_float() ==
            attr2_value);

    // Make sure the attached component is restored with correct attribute values
    REQUIRE(map_ctx.get_component(component_id).get_attr(attr1_name).as_string() ==
            new_attr1_value);
    REQUIRE(map_ctx.get_component(component_id).get_attr(attr2_name).as_float() ==
            new_attr2_value);
  }
}

}  // namespace tactile::test
