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

#include "common/debug/assert.hpp"
#include "model/contexts/context_components.hpp"
#include "model/entity_validation.hpp"

namespace tactile::sys {

auto copy_component(Model& model, const AttachedComponent& src_component) -> Entity
{
  const auto new_component_entity = model.create_entity();

  auto& new_component = model.add<AttachedComponent>(new_component_entity);
  new_component = src_component;

  TACTILE_ASSERT(is_attached_component_entity(model, new_component_entity));
  return new_component_entity;
}

auto reset_component_values(const Model& model, AttachedComponent& component)
    -> StringMap<Attribute>
{
  const auto& definition = model.get<ComponentDefinition>(component.definition);

  auto previous_values = std::move(component.attributes);

  component.attributes.clear();
  component.attributes = definition.attributes;

  return previous_values;
}

}  // namespace tactile::sys