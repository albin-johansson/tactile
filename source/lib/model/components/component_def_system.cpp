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

#include "common/util/lookup.hpp"
#include "core/debug/assert.hpp"
#include "model/components/component_components.hpp"
#include "model/entity_validation.hpp"

namespace tactile::sys {

auto instantiate_component(Registry& registry, const Entity component_entity) -> Entity
{
  TACTILE_ASSERT(is_component_entity(registry, component_entity));
  const auto& component = registry.get<Component>(component_entity);

  const auto attached_component_entity = registry.create_entity();

  auto& attached_component = registry.add<AttachedComponent>(attached_component_entity);
  attached_component.definition = component_entity;
  attached_component.attributes = component.attributes;

  TACTILE_ASSERT(is_attached_component_entity(registry, attached_component_entity));
  return attached_component_entity;
}

void add_component_attribute(Registry& registry,
                             const Entity component_entity,
                             const String& name,
                             const Attribute& value)
{
  TACTILE_ASSERT(is_component_entity(registry, component_entity));

  auto& component = registry.get<Component>(component_entity);
  component.attributes[name] = value;

  for (auto [attached_component_entity, attached_component]:
       registry.each<AttachedComponent>()) {
    if (attached_component.definition == component_entity) {
      attached_component.attributes[name] = value;
    }
  }
}

void remove_component_attribute(Registry& registry,
                                const Entity component_entity,
                                StringView attribute_name)
{
  TACTILE_ASSERT(is_component_entity(registry, component_entity));
  auto& component = registry.get<Component>(component_entity);

  // Removes the attribute from all component instances of the affected type
  if (erase_from(component.attributes, attribute_name).succeeded()) {
    for (auto [attached_component_entity, attached_component]:
         registry.each<AttachedComponent>()) {
      if (attached_component.definition == component_entity) {
        erase_from(attached_component.attributes, attribute_name);
      }
    }
  }
}

void rename_component_attribute(Registry& registry,
                                const Entity component_entity,
                                StringView old_attribute_name,
                                const String& new_attribute_name)
{
  TACTILE_ASSERT(is_component_entity(registry, component_entity));

  auto& component = registry.get<Component>(component_entity);
  const auto attribute_value = lookup_in(component.attributes, old_attribute_name);

  erase_from(component.attributes, old_attribute_name);
  component.attributes[new_attribute_name] = attribute_value;

  for (auto [attached_component_entity, attached_component]:
       registry.each<AttachedComponent>()) {
    if (attached_component.definition == component_entity) {
      erase_from(attached_component.attributes, old_attribute_name);
      attached_component.attributes[new_attribute_name] = attribute_value;
    }
  }
}

auto duplicate_component_attribute(Registry& registry,
                                   const Entity component_entity,
                                   StringView attribute_name) -> String
{
  TACTILE_ASSERT(is_component_entity(registry, component_entity));

  auto& component = registry.get<Component>(component_entity);
  const auto& attribute_value = lookup_in(component.attributes, attribute_name);

  int suffix = 1;
  String new_attribute_name;
  do {
    new_attribute_name = fmt::format("{} ({})", attribute_name, suffix);
    ++suffix;
  } while (has_key(component.attributes, new_attribute_name));

  add_component_attribute(registry,
                          component_entity,
                          new_attribute_name,
                          attribute_value);
  return new_attribute_name;
}

void force_update_component_attribute(Registry& registry,
                                      const Entity component_entity,
                                      const String& attribute_name,
                                      const Attribute& attribute_value)
{
  TACTILE_ASSERT(is_component_entity(registry, component_entity));

  auto& component = registry.get<Component>(component_entity);
  component.attributes[attribute_name] = attribute_value;

  for (auto [attached_component_entity, attached_component]:
       registry.each<AttachedComponent>()) {
    if (attached_component.definition == component_entity) {
      attached_component.attributes[attribute_name] = attribute_value;
    }
  }
}

}  // namespace tactile::sys
