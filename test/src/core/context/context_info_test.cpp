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

#include "core/context/context_info.hpp"

#include <doctest/doctest.h>

#include "core/component/component.hpp"
#include "core/component/component_definition.hpp"

namespace tactile::test {

TEST_SUITE("ContextInfo")
{
  TEST_CASE("defaults")
  {
    const ContextInfo ctx;
    REQUIRE(!ctx.get_uuid().is_nil());
    REQUIRE(ctx.name().empty());
    REQUIRE(ctx.component_count() == 0u);
  }

  TEST_CASE("ID constructor")
  {
    const auto id = make_uuid();
    const ContextInfo ctx {id};
    REQUIRE(ctx.get_uuid() == id);
  }

  TEST_CASE("set_name")
  {
    ContextInfo ctx;
    ctx.set_name("foo");
    REQUIRE(ctx.name() == "foo");
  }

  TEST_CASE("attach_component")
  {
    ContextInfo ctx;
    const ComponentDefinition component_def;

    REQUIRE(ctx.attach_component(component_def.instantiate()).succeeded());
    REQUIRE(ctx.has_component(component_def.get_uuid()));
    REQUIRE(ctx.component_count() == 1u);
  }

  TEST_CASE("detach_component")
  {
    ContextInfo ctx;

    ComponentDefinition component_def;
    component_def.add("A", 1);
    component_def.add("B", 2);

    const auto component_id = component_def.get_uuid();
    REQUIRE(ctx.attach_component(component_def.instantiate()).succeeded());
    REQUIRE(ctx.has_component(component_id));

    const auto removed_component = ctx.detach_component(component_id);
    REQUIRE(removed_component.has_value());
    REQUIRE(!ctx.has_component(component_id));
    REQUIRE(ctx.component_count() == 0u);

    REQUIRE(removed_component->size() == component_def.size());
    REQUIRE(removed_component->definition_id() == component_def.get_uuid());
  }

  TEST_CASE("get_component")
  {
    ContextInfo ctx;
    const ComponentDefinition component_def;

    REQUIRE(ctx.attach_component(component_def.instantiate()).succeeded());

    const auto& component = ctx.get_component(component_def.get_uuid());
    REQUIRE_NOTHROW(component.definition_id() == component_def.get_uuid());
  }

  TEST_CASE("find_component")
  {
    ContextInfo ctx;
    const ComponentDefinition component_def;

    REQUIRE(ctx.find_component(component_def.get_uuid()) == nullptr);
    REQUIRE(ctx.attach_component(component_def.instantiate()).succeeded());
    REQUIRE(ctx.find_component(component_def.get_uuid()) != nullptr);
  }

  TEST_CASE("has_component")
  {
    ContextInfo ctx;
    const ComponentDefinition component_def;

    REQUIRE(!ctx.has_component(component_def.get_uuid()));
    REQUIRE(ctx.attach_component(component_def.instantiate()).succeeded());
    REQUIRE(ctx.has_component(component_def.get_uuid()));
  }
}

}  // namespace tactile::test