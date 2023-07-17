/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "component_builder.hpp"

#include <utility>  // move

#include "model/components/component_components.hpp"
#include "model/components/component_factory.hpp"

namespace tactile {

ComponentBuilder::ComponentBuilder(Registry& registry, const Entity component_set_entity)
    : mRegistry {&registry},
      mComponentSetEntity {component_set_entity}
{
}

auto ComponentBuilder::with_name(String name) -> Self&
{
  mComponentName = std::move(name);
  return *this;
}

auto ComponentBuilder::with_attribute(String name, Attribute value) -> Self&
{
  mAttributes[std::move(name)] = std::move(value);
  return *this;
}

auto ComponentBuilder::build() -> Entity
{
  auto& component_set = mRegistry->get<ComponentSet>(mComponentSetEntity);
  const auto component_entity =
      sys::create_component(*mRegistry, component_set, mComponentName);

  auto& component = mRegistry->get<Component>(component_entity);
  component.attributes = std::move(mAttributes);

  return component_entity;
}

}  // namespace tactile
