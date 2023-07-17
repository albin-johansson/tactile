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

#include "component_factory.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "model/entity_validation.hpp"

namespace tactile::sys {

auto create_component(Registry& registry, ComponentSet& component_set, String name)
    -> Entity
{
  const auto component_entity = registry.create_entity();
  component_set.definitions.push_back(component_entity);

  auto& component = registry.add<Component>(component_entity);
  component.name = std::move(name);

  TACTILE_ASSERT(is_component_entity(registry, component_entity));
  return component_entity;
}

}  // namespace tactile::sys
