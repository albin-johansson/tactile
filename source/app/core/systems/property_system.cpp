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

#include "property_system.hpp"

#include <utility>  // move
#include <vector>   // erase

#include <entt/entity/registry.hpp>

#include "core/common/ecs.hpp"
#include "core/systems/context_system.hpp"
#include "misc/assert.hpp"

namespace tactile::sys {

void add_property(entt::registry& registry,
                  comp::AttributeContext& context,
                  std::string name,
                  const AttributeType type)
{
  TACTILE_ASSERT(!has_property_with_name(registry, context, name));

  const auto entity = registry.create();

  auto& property = registry.emplace<comp::Property>(entity);
  property.name = std::move(name);
  property.value.reset_to_default(type);

  context.properties.push_back(entity);
}

void add_property(entt::registry& registry,
                  comp::AttributeContext& context,
                  std::string name,
                  Attribute value)
{
  TACTILE_ASSERT(!has_property_with_name(registry, context, name));

  const auto entity = registry.create();

  auto& property = registry.emplace<comp::Property>(entity);
  property.name = std::move(name);
  property.value = std::move(value);

  context.properties.push_back(entity);
}

void remove_property(entt::registry& registry,
                     comp::AttributeContext& context,
                     const std::string_view name)
{
  const auto entity = find_property(registry, context, name);
  TACTILE_ASSERT(entity != entt::null);

  std::erase(context.properties, entity);
  destroy_entity(registry, entity);
}

void rename_property(entt::registry& registry,
                     comp::AttributeContext& context,
                     const std::string_view oldName,
                     std::string newName)
{
  TACTILE_ASSERT(has_property_with_name(registry, context, oldName));
  TACTILE_ASSERT(!has_property_with_name(registry, context, newName));

  const auto entity = find_property(registry, context, oldName);
  auto& property = checked_get<comp::Property>(registry, entity);
  property.name = std::move(newName);
}

void update_property(entt::registry& registry,
                     comp::AttributeContext& context,
                     const std::string_view name,
                     Attribute value)
{
  const auto entity = find_property(registry, context, name);
  auto& property = checked_get<comp::Property>(registry, entity);
  property.value = std::move(value);
}

void change_property_type(entt::registry& registry,
                          comp::AttributeContext& context,
                          const std::string_view name,
                          const AttributeType type)
{
  const auto entity = find_property(registry, context, name);
  auto& property = checked_get<comp::Property>(registry, entity);
  property.value.reset_to_default(type);
}

auto find_property(const entt::registry& registry,
                   const comp::AttributeContext& context,
                   const std::string_view name) -> entt::entity
{
  for (const auto entity : context.properties) {
    const auto& property = checked_get<comp::Property>(registry, entity);
    if (property.name == name) {
      return entity;
    }
  }

  return entt::null;
}

auto get_property(const entt::registry& registry,
                  const comp::AttributeContext& context,
                  const std::string_view name) -> const comp::Property&
{
  const auto entity = find_property(registry, context, name);
  return checked_get<comp::Property>(registry, entity);
}

auto has_property_with_name(const entt::registry& registry,
                            const comp::AttributeContext& context,
                            const std::string_view name) -> bool
{
  return find_property(registry, context, name) != entt::null;
}

}  // namespace tactile::sys
