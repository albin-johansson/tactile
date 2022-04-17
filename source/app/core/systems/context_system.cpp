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

#include <entt/entt.hpp>

#include "component_system.hpp"
#include "misc/assert.hpp"
#include "misc/throw.hpp"
#include "property_system.hpp"
#include "registry_system.hpp"

namespace tactile::sys {
namespace {

/* Identifier used to distinguish attribute contexts, generated on a
   session-by-session basis and not stored anywhere in save files. */
inline ContextID _next_context_id{1};

}  // namespace

void reset_next_context_id() noexcept
{
  _next_context_id = 1;
}

auto get_and_update_next_context_id() noexcept -> ContextID
{
  return _next_context_id++;
}

auto next_context_id() noexcept -> ContextID
{
  return _next_context_id;
}

auto add_attribute_context(entt::registry& registry, const entt::entity entity)
    -> comp::AttributeContext&
{
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.emplace<comp::AttributeContext>(entity);
  context.id = get_and_update_next_context_id();

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

auto get_context(entt::registry& registry, const ContextID id) -> comp::AttributeContext&
{
  if (auto& context = registry.ctx<comp::AttributeContext>(); context.id == id) {
    return context;
  }

  for (auto&& [entity, context] : registry.view<comp::AttributeContext>().each()) {
    if (context.id == id) {
      return context;
    }
  }

  panic("No matching attribute context!");
}

auto get_context(const entt::registry& registry, const ContextID id)
    -> const comp::AttributeContext&
{
  if (const auto& context = registry.ctx<comp::AttributeContext>(); context.id == id) {
    return context;
  }

  for (auto&& [entity, context] : registry.view<comp::AttributeContext>().each()) {
    if (context.id == id) {
      return context;
    }
  }

  panic("No matching attribute context!");
}

auto current_context(const entt::registry& registry) -> const comp::AttributeContext&
{
  const auto& current = registry.ctx<comp::ActiveAttributeContext>();
  return (current.entity != entt::null)
             ? checked_get<comp::AttributeContext>(registry, current.entity)
             : registry.ctx<comp::AttributeContext>();
}

auto current_context_id(const entt::registry& registry) -> ContextID
{
  const auto& context = current_context(registry);
  return context.id;
}

}  // namespace tactile::sys