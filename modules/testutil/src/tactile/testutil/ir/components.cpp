// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/testutil/ir/components.hpp"

#include <utility>  // move

#include "tactile/testutil/ir/properties.hpp"

namespace tactile::testutil {

auto make_empty_ir_component(String name) -> ir::Component
{
  return {
    .name = std::move(name),
    .attributes = Vector<ir::NamedAttribute> {},
  };
}

auto make_ir_component_with_basic_properties(String name) -> ir::Component
{
  auto component = make_empty_ir_component(std::move(name));
  component.attributes = make_basic_properties();

  return component;
}

auto make_ir_component_with_all_properties(String name) -> ir::Component
{
  auto component = make_empty_ir_component(std::move(name));
  component.attributes = make_all_properties();

  return component;
}

auto make_ir_attached_component(const ir::Component& prototype) -> ir::AttachedComponent
{
  return {prototype.name, prototype.attributes};
}

}  // namespace tactile::testutil
