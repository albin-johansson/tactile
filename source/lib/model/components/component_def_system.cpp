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

#include "component_def_system.hpp"

#include <fmt/format.h>

#include "common/debug/assert.hpp"
#include "common/util/lookup.hpp"
#include "model/components/component_components.hpp"
#include "model/entity_validation.hpp"

namespace tactile::sys {

auto instantiate_component(Model& model, const Entity definition_entity) -> Entity
{
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));
  const auto& definition = model.get<ComponentDefinition>(definition_entity);

  const auto attached_component_entity = model.create_entity();

  auto& attached_component = model.add<AttachedComponent>(attached_component_entity);
  attached_component.definition = definition_entity;
  attached_component.attributes = definition.attributes;

  TACTILE_ASSERT(is_attached_component_entity(model, attached_component_entity));
  return attached_component_entity;
}

void add_component_attribute(Model& model,
                             const Entity definition_entity,
                             const String& name,
                             const Attribute& value)
{
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));

  auto& definition = model.get<ComponentDefinition>(definition_entity);
  definition.attributes[name] = value;

  for (auto [attached_component_entity, attached_component]:
       model.each<AttachedComponent>()) {
    if (attached_component.definition == definition_entity) {
      attached_component.attributes[name] = value;
    }
  }
}

void remove_component_attribute(Model& model,
                                const Entity definition_entity,
                                StringView attribute_name)
{
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));
  auto& definition = model.get<ComponentDefinition>(definition_entity);

  // Removes the attribute from all component instances of the affected type
  if (erase_from(definition.attributes, attribute_name).succeeded()) {
    for (auto [attached_component_entity, attached_component]:
         model.each<AttachedComponent>()) {
      if (attached_component.definition == definition_entity) {
        erase_from(attached_component.attributes, attribute_name);
      }
    }
  }
}

void rename_component_attribute(Model& model,
                                const Entity definition_entity,
                                StringView old_attribute_name,
                                const String& new_attribute_name)
{
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));

  auto& definition = model.get<ComponentDefinition>(definition_entity);
  const auto attribute_value = lookup_in(definition.attributes, old_attribute_name);

  erase_from(definition.attributes, old_attribute_name);
  definition.attributes[new_attribute_name] = attribute_value;

  for (auto [attached_component_entity, attached_component]:
       model.each<AttachedComponent>()) {
    if (attached_component.definition == definition_entity) {
      erase_from(attached_component.attributes, old_attribute_name);
      attached_component.attributes[new_attribute_name] = attribute_value;
    }
  }
}

auto duplicate_component_attribute(Model& model,
                                   const Entity definition_entity,
                                   StringView attribute_name) -> String
{
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));

  auto& definition = model.get<ComponentDefinition>(definition_entity);
  const auto& attribute_value = lookup_in(definition.attributes, attribute_name);

  int suffix = 1;
  String new_attribute_name;
  do {
    new_attribute_name = fmt::format("{} ({})", attribute_name, suffix);
    ++suffix;
  } while (has_key(definition.attributes, new_attribute_name));

  add_component_attribute(model, definition_entity, new_attribute_name, attribute_value);
  return new_attribute_name;
}

void force_update_component_attribute(Model& model,
                                      const Entity definition_entity,
                                      const String& attribute_name,
                                      const Attribute& attribute_value)
{
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));

  auto& definition = model.get<ComponentDefinition>(definition_entity);
  definition.attributes[attribute_name] = attribute_value;

  for (auto [attached_component_entity, attached_component]:
       model.each<AttachedComponent>()) {
    if (attached_component.definition == definition_entity) {
      attached_component.attributes[attribute_name] = attribute_value;
    }
  }
}

}  // namespace tactile::sys
