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

#include "component_system.hpp"

#include <algorithm>  // find_if
#include <utility>    // move

#include <fmt/format.h>

#include "common/debug/assert.hpp"
#include "common/util/assoc.hpp"
#include "core/context.hpp"
#include "model/systems/context_system.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _find_component_definition(const Model& model,
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

[[nodiscard]] auto _find_component_of_type(const Model& model,
                                           const Vec<Entity>& components,
                                           const Entity definition_entity)
{
  auto predicate = [&](const Entity component_entity) {
    const auto& component = model.get<Component>(component_entity);
    return component.definition == definition_entity;
  };

  return std::find_if(components.begin(), components.end(), predicate);
}

[[nodiscard]] auto _has_component_type(const Model& model,
                                       const Context& context,
                                       const Entity definition_entity) -> bool
{
  const auto iter = _find_component_of_type(model, context.comps, definition_entity);
  return iter != context.comps.end();
}

}  // namespace

auto is_component_set_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity && model.has<ComponentSet>(entity);
}

auto is_component_definition_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity && model.has<ComponentDefinition>(entity);
}

auto is_component_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity && model.has<Component>(entity);
}

auto create_component(Model& model, const Entity component_set_entity, String name)
    -> Entity
{
  TACTILE_ASSERT(is_component_set_entity(model, component_set_entity));
  const auto definition_entity = model.create_entity();

  auto& definition = model.add<ComponentDefinition>(definition_entity);
  definition.name = std::move(name);

  auto& component_set = model.get<ComponentSet>(component_set_entity);
  component_set.definitions.push_back(definition_entity);

  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));
  return definition_entity;
}

void remove_component(Model& model, const Entity component_set_entity, StringView name)
{
  TACTILE_ASSERT(is_component_set_entity(model, component_set_entity));
  auto& component_set = model.get<ComponentSet>(component_set_entity);

  const auto definition_entity = _find_component_definition(model, component_set, name);
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

auto instantiate_component(Model& model, const Entity definition_entity) -> Entity
{
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));
  const auto& definition = model.get<ComponentDefinition>(definition_entity);

  const auto component_entity = model.create_entity();

  auto& component = model.add<Component>(component_entity);
  component.definition = definition_entity;
  component.attributes = definition.attributes;

  TACTILE_ASSERT(is_component_entity(model, component_entity));
  return component_entity;
}

auto copy_component(Model& model, const Entity component_entity) -> Entity
{
  TACTILE_ASSERT(is_component_entity(model, component_entity));
  const auto& src_component = model.get<Component>(component_entity);

  const auto new_component_entity = model.create_entity();

  auto& new_component = model.add<Component>(new_component_entity);
  new_component = src_component;

  return new_component_entity;
}

void add_component_attribute(Model& model,
                             const Entity definition_entity,
                             const String& name,
                             const Attribute& value)
{
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));

  auto& definition = model.get<ComponentDefinition>(definition_entity);
  definition.attributes[name] = value;

  for (auto [component_entity, component]: model.each<Component>()) {
    if (component.definition == definition_entity) {
      component.attributes[name] = value;
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
    for (auto [component_entity, component]: model.each<Component>()) {
      if (component.definition == definition_entity) {
        erase_from(component.attributes, attribute_name);
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

  for (auto [component_entity, component]: model.each<Component>()) {
    if (component.definition == definition_entity) {
      erase_from(component.attributes, old_attribute_name);
      component.attributes[new_attribute_name] = attribute_value;
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

  for (auto [component_entity, component]: model.each<Component>()) {
    if (component.definition == definition_entity) {
      component.attributes[attribute_name] = attribute_value;
    }
  }
}

auto reset_component_values(Model& model, const Entity component_entity)
    -> StringMap<Attribute>
{
  TACTILE_ASSERT(is_component_entity(model, component_entity));

  auto& component = model.get<Component>(component_entity);
  const auto& definition = model.get<ComponentDefinition>(component.definition);

  auto previous_values = std::move(component.attributes);

  component.attributes.clear();
  component.attributes = definition.attributes;

  return previous_values;
}

auto attach_component(Model& model,
                      const Entity context_entity,
                      const Entity definition_entity) -> Entity
{
  TACTILE_ASSERT(is_context_entity(model, context_entity));
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));

  auto& context = model.get<Context>(context_entity);
  TACTILE_ASSERT(!_has_component_type(model, context, definition_entity));

  const auto component_entity = instantiate_component(model, definition_entity);
  context.comps.push_back(component_entity);

  return component_entity;
}

auto detach_component(Model& model,
                      const Entity context_entity,
                      const Entity definition_entity) -> Maybe<StringMap<Attribute>>
{
  TACTILE_ASSERT(is_context_entity(model, context_entity));
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));

  auto& context = model.get<Context>(context_entity);
  const auto iter = _find_component_of_type(model, context.comps, definition_entity);

  if (iter != context.comps.end()) {
    auto values = model.get<Component>(*iter).attributes;

    context.comps.erase(iter);
    model.destroy(*iter);

    return values;
  }

  return nothing;
}

auto copy_single_attribute_in_components(const Model& model,
                                         const ComponentDefinitionEntity def_entity,
                                         StringView attribute_name)
    -> HashMap<ComponentEntity, Attribute>
{
  TACTILE_ASSERT(is_component_definition_entity(model, def_entity));

  HashMap<ComponentEntity, Attribute> attributes;

  for (auto [component_entity, component]: model.each<Component>()) {
    if (component.definition == def_entity) {
      attributes[component_entity] = lookup_in(component.attributes, attribute_name);
    }
  }

  return attributes;
}

auto copy_component_values(const Model& model,
                           const ComponentDefinitionEntity definition_entity)
    -> HashMap<ContextEntity, StringMap<Attribute>>
{
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));
  HashMap<ContextEntity, StringMap<Attribute>> component_values;

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

}  // namespace tactile::sys