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

#include "component_set_system.hpp"

#include "common/debug/assert.hpp"
#include "model/contexts/context_components.hpp"
#include "model/entity_validation.hpp"

namespace tactile::sys {

auto create_component_set(Model& model) -> Entity
{
  const auto component_set_entity = model.create_entity();

  model.add<ComponentSet>(component_set_entity);

  TACTILE_ASSERT(is_component_set_entity(model, component_set_entity));
  return component_set_entity;
}

void remove_component(Model& model, ComponentSet& component_set, StringView name)
{
  const auto definition_entity = find_component(model, component_set, name);
  if (definition_entity == kNullEntity) {
    return;
  }

  std::erase(component_set.definitions, definition_entity);

  for (auto [entity, context]: model.each<Context>()) {
    std::erase_if(context.comps, [&](const Entity attached_component_entity) {
      const auto& attached_component =
          model.get<AttachedComponent>(attached_component_entity);

      if (attached_component.definition == definition_entity) {
        model.destroy(attached_component_entity);
        return true;
      }

      return false;
    });
  }
}

auto find_component(const Model& model,
                    const ComponentSet& component_set,
                    StringView name) -> Entity
{
  for (const auto component_entity: component_set.definitions) {
    const auto& component = model.get<Component>(component_entity);

    if (component.name == name) {
      return component_entity;
    }
  }

  return kNullEntity;
}

}  // namespace tactile::sys
