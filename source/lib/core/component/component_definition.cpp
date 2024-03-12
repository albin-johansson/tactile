// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "component_definition.hpp"

#include <utility>  // move

namespace tactile {

ComponentDefinition::ComponentDefinition(const UUID& id)
    : ComponentBase {id}
{
}

void ComponentDefinition::set_name(String name)
{
  mName = std::move(name);
}

auto ComponentDefinition::get_name() const -> const String&
{
  return mName;
}

auto ComponentDefinition::get_uuid() const -> const UUID&
{
  return mTypeId;
}

auto ComponentDefinition::instantiate() const -> Component
{
  return {mTypeId, mAttributes};
}

}  // namespace tactile
