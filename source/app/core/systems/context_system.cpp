/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include <utility>  // move

#include <entt/entity/registry.hpp>

#include "core/common/ecs.hpp"
#include "core/components/contexts.hpp"
#include "core/systems/component_system.hpp"
#include "core/systems/property_system.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile::sys {
namespace {

void _unregister_context(entt::registry& registry, const UUID& id)
{
  auto& mapping = ctx_get<comp::ContextMapping>(registry);
  const auto count = mapping.entities.erase(id);
  if (count == 0) {
    throw TactileError{"Invalid context identifier!"};
  }
}

}  // namespace

void destroy_entity(entt::registry& registry, const entt::entity entity)
{
  if (auto* context = registry.try_get<comp::AttributeContext>(entity)) {
    _unregister_context(registry, context->id);
  }

  registry.destroy(entity);
}

auto register_context(entt::registry& registry, const entt::entity entity)
    -> comp::AttributeContext&
{
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.emplace<comp::AttributeContext>(entity);
  context.id = make_uuid();

  auto& mapping = ctx_get<comp::ContextMapping>(registry);
  mapping.entities[context.id] = entity;

  return context;
}

auto copy_attribute_context(const entt::registry& registry, const entt::entity source)
    -> AttributeContextSnapshot
{
  const auto& context = checked_get<comp::AttributeContext>(registry, source);

  AttributeContextSnapshot snapshot;
  snapshot.id = context.id;
  snapshot.name = context.name;

  for (const auto propertyEntity : context.properties) {
    const auto& property = checked_get<comp::Property>(registry, propertyEntity);
    snapshot.properties.try_emplace(property.name, property.value);
  }

  for (const auto componentEntity : context.components) {
    const auto& component = checked_get<comp::Component>(registry, componentEntity);

    auto& componentSnapshot = snapshot.components[component.type];
    for (const auto& [attrName, attrValue] : component.values) {
      componentSnapshot.attributes[attrName] = attrValue;
    }
  }

  return snapshot;
}

void restore_attribute_context(entt::registry& registry,
                               const entt::entity entity,
                               AttributeContextSnapshot snapshot)
{
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get_or_emplace<comp::AttributeContext>(entity);
  context.id = snapshot.id;
  context.name = std::move(snapshot.name);

  auto& mapping = ctx_get<comp::ContextMapping>(registry);
  TACTILE_ASSERT(!mapping.entities.contains(context.id));

  mapping.entities[context.id] = entity;

  for (auto&& [propertyName, propertyValue] : snapshot.properties) {
    add_property(registry, context, propertyName, propertyValue);
  }

  for (auto&& [componentId, componentValue] : snapshot.components) {
    auto& component = add_component(registry, context.id, componentId);

    for (const auto& [attrName, attrValue] : componentValue.attributes) {
      component.values.at(attrName) = attrValue;
    }
  }
}

void set_context_id(entt::registry& registry,
                    comp::AttributeContext& context,
                    const UUID& id)
{
  auto& mapping = ctx_get<comp::ContextMapping>(registry);
  const auto entity = mapping.entities.at(context.id);

  mapping.entities.erase(context.id);
  mapping.entities[id] = entity;

  context.id = id;
}

auto find_context(const entt::registry& registry, const UUID& id) -> entt::entity
{
  const auto& mapping = ctx_get<comp::ContextMapping>(registry);
  if (const auto iter = mapping.entities.find(id); iter != mapping.entities.end()) {
    return iter->second;
  }
  else {
    return entt::null;
  }
}

auto get_context(entt::registry& registry, const UUID& id) -> comp::AttributeContext&
{
  if (auto& context = ctx_get<comp::AttributeContext>(registry); context.id == id) {
    return context;
  }

  for (auto&& [entity, context] : registry.view<comp::AttributeContext>().each()) {
    if (context.id == id) {
      return context;
    }
  }

  throw TactileError{"No matching attribute context!"};
}

auto get_context(const entt::registry& registry, const UUID& id)
    -> const comp::AttributeContext&
{
  if (const auto& context = ctx_get<comp::AttributeContext>(registry); context.id == id) {
    return context;
  }

  for (auto&& [entity, context] : registry.view<comp::AttributeContext>().each()) {
    if (context.id == id) {
      return context;
    }
  }

  throw TactileError{"No matching attribute context!"};
}

auto current_context(const entt::registry& registry) -> const comp::AttributeContext&
{
  const auto& current = ctx_get<comp::ActiveAttributeContext>(registry);
  return (current.entity != entt::null)
             ? checked_get<comp::AttributeContext>(registry, current.entity)
             : ctx_get<comp::AttributeContext>(registry);
}

}  // namespace tactile::sys