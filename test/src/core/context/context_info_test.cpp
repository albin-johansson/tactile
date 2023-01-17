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
    REQUIRE(ctx.property_count() == 0u);
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

  TEST_CASE("add_property (with type)")
  {
    ContextInfo ctx;
    const String property_name {"foo"};

    REQUIRE(ctx.add_property(property_name, AttributeType::Int).succeeded());
    REQUIRE(ctx.property_count() == 1u);
    REQUIRE(ctx.has_property(property_name));
    REQUIRE(ctx.get_property(property_name).as_int() == 0);

    REQUIRE(ctx.add_property(property_name, AttributeType::Float).failed());
    REQUIRE(ctx.property_count() == 1u);
    REQUIRE(ctx.has_property(property_name));
    REQUIRE(ctx.get_property(property_name).get_type() == AttributeType::Int);
  }

  TEST_CASE("add_property (with value)")
  {
    ContextInfo ctx;
    const String property_name {"foo"};
    const String property_value {"bar"};

    REQUIRE(ctx.add_property(property_name, property_value).succeeded());
    REQUIRE(ctx.has_property(property_name));
    REQUIRE(ctx.get_property(property_name) == property_value);

    REQUIRE(ctx.add_property(property_name, 42).failed());
    REQUIRE(ctx.property_count() == 1u);
  }

  TEST_CASE("update_property")
  {
    ContextInfo ctx;
    const String property_name {"A"};

    REQUIRE(ctx.update_property(property_name, 10).failed());

    REQUIRE(ctx.add_property(property_name, 10).succeeded());
    REQUIRE(ctx.property_count() == 1u);

    REQUIRE(ctx.update_property(property_name, true).succeeded());
    REQUIRE(ctx.get_property(property_name) == true);
    REQUIRE(ctx.property_count() == 1u);

    REQUIRE(ctx.update_property(property_name, 4.5f).succeeded());
    REQUIRE(ctx.get_property(property_name) == 4.5f);
    REQUIRE(ctx.property_count() == 1u);
  }

  TEST_CASE("remove_property")
  {
    ContextInfo ctx;
    const String property_name {"abc"};

    REQUIRE(!ctx.remove_property(property_name).has_value());

    REQUIRE(ctx.add_property(property_name, 123).succeeded());
    REQUIRE(ctx.has_property(property_name));

    REQUIRE(ctx.remove_property(property_name) == 123);
    REQUIRE(!ctx.has_property(property_name));
  }

  TEST_CASE("rename_property")
  {
    ContextInfo ctx;
    const String old_property_name {"foo"};
    const String new_property_name {"bar"};

    REQUIRE(ctx.rename_property(old_property_name, new_property_name).failed());

    REQUIRE(ctx.add_property(old_property_name, 42).succeeded());
    REQUIRE(ctx.has_property(old_property_name));
    REQUIRE(!ctx.has_property(new_property_name));

    REQUIRE(ctx.rename_property(old_property_name, new_property_name).succeeded());
    REQUIRE(!ctx.has_property(old_property_name));
    REQUIRE(ctx.has_property(new_property_name));

    REQUIRE(ctx.property_count() == 1u);
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
    component_def.add_attr("A", 1);
    component_def.add_attr("B", 2);

    const auto component_id = component_def.get_uuid();
    REQUIRE(ctx.attach_component(component_def.instantiate()).succeeded());
    REQUIRE(ctx.has_component(component_id));

    const auto removed_component = ctx.detach_component(component_id);
    REQUIRE(removed_component.has_value());
    REQUIRE(!ctx.has_component(component_id));
    REQUIRE(ctx.component_count() == 0u);

    REQUIRE(removed_component->attr_count() == component_def.attr_count());
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