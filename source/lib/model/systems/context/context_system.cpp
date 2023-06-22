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

#include "context_system.hpp"

#include "common/debug/assert.hpp"
#include "common/util/lookup.hpp"
#include "model/components/component_components.hpp"
#include "model/systems/component/component.hpp"
#include "model/systems/validation_system.hpp"

namespace tactile::sys {

auto copy_context(Model& model, const Context& src_context) -> Context
{
  Context new_context;
  new_context.name = src_context.name;
  new_context.props = src_context.props;

  new_context.comps.reserve(src_context.comps.size());
  for (const auto src_component_entity: src_context.comps) {
    const auto& src_component = model.get<Component>(src_component_entity);
    new_context.comps.push_back(copy_component(model, src_component));
  }

  return new_context;
}

auto copy_component_values(const Model& model, const Entity definition_entity)
    -> HashMap<Entity, StringMap<Attribute>>
{
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));
  HashMap<Entity, StringMap<Attribute>> component_values;

  for (auto [context_entity, context]: model.each<Context>()) {
    for (const auto component_entity: context.comps) {
      const auto& component = model.get<Component>(component_entity);

      if (component.definition == definition_entity) {
        component_values[context_entity] = component.attributes;
      }
    }
  }

  return component_values;
}

auto copy_single_attribute_in_components(const Model& model,
                                         const Entity definition_entity,
                                         StringView attribute_name)
    -> HashMap<Entity, Attribute>
{
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));
  HashMap<Entity, Attribute> attributes;

  for (auto [component_entity, component]: model.each<Component>()) {
    if (component.definition == definition_entity) {
      attributes[component_entity] = lookup_in(component.attributes, attribute_name);
    }
  }

  return attributes;
}

}  // namespace tactile::sys
