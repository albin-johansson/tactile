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

#include "component_set.hpp"

#include "common/debug/assert.hpp"
#include "core/context.hpp"
#include "model/systems/validation.hpp"

namespace tactile::sys {

auto create_component(Model& model, ComponentSet& component_set, String name) -> Entity
{
  const auto definition_entity = model.create_entity();
  component_set.definitions.push_back(definition_entity);

  auto& definition = model.add<ComponentDefinition>(definition_entity);
  definition.name = std::move(name);

  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));
  return definition_entity;
}

void remove_component(Model& model, ComponentSet& component_set, StringView name)
{
  const auto definition_entity = find_component_definition(model, component_set, name);
  if (definition_entity == kNullEntity) {
    return;
  }

  std::erase(component_set.definitions, definition_entity);

  for (auto [entity, context]: model.each<Context>()) {
    std::erase_if(context.comps, [&](const Entity component_entity) {
      const auto& component = model.get<Component>(component_entity);

      if (component.definition == definition_entity) {
        model.destroy(component_entity);
        return true;
      }

      return false;
    });
  }
}

auto find_component_definition(const Model& model,
                               const ComponentSet& component_set,
                               StringView name) -> Entity
{
  for (const auto definition_entity: component_set.definitions) {
    const auto& definition = model.get<ComponentDefinition>(definition_entity);

    if (definition.name == name) {
      return definition_entity;
    }
  }

  return kNullEntity;
}

auto find_component_definition(const Model& model,
                               const Entity component_set_entity,
                               StringView name) -> Entity
{
  TACTILE_ASSERT(is_component_set_entity(model, component_set_entity));

  const auto& component_set = model.get<ComponentSet>(component_set_entity);
  return find_component_definition(model, component_set, name);
}

}  // namespace tactile::sys
