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

#include "attached_component_system.hpp"

#include <utility>  // move

#include "core/debug/assert.hpp"
#include "model/contexts/context_components.hpp"
#include "model/entity_validation.hpp"

namespace tactile::sys {

auto copy_component(Registry& registry, const AttachedComponent& src_attached_component)
    -> Entity
{
  const auto new_attached_component_entity = registry.create_entity();

  auto& new_attached_component =
      registry.add<AttachedComponent>(new_attached_component_entity);
  new_attached_component = src_attached_component;

  TACTILE_ASSERT(is_attached_component_entity(registry, new_attached_component_entity));
  return new_attached_component_entity;
}

auto reset_component_values(const Registry& registry,
                            AttachedComponent& attached_component) -> StringMap<Attribute>
{
  const auto& component = registry.get<Component>(attached_component.definition);

  auto previous_values = std::move(attached_component.attributes);

  attached_component.attributes.clear();
  attached_component.attributes = component.attributes;

  return previous_values;
}

}  // namespace tactile::sys