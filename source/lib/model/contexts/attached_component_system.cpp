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

#include <algorithm>  // find_if, any_of

#include "model/components/component_def_system.hpp"
#include "model/entity_validation.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _find_component_of_type(const Registry& registry,
                                           const Vector<Entity>& component_entities,
                                           const Entity definition_entity)
    -> Vector<Entity>::const_iterator
{
  return std::ranges::find_if(
      component_entities,
      [&](const Entity attached_component_entity) {
        const auto& attached_component =
            registry.get<AttachedComponent>(attached_component_entity);
        return attached_component.definition == definition_entity;
      });
}

[[nodiscard]] auto _has_component_type(const Registry& registry,
                                       const Context& context,
                                       const Entity definition_entity) -> bool
{
  const auto iter = _find_component_of_type(registry, context.comps, definition_entity);
  return iter != context.comps.end();
}

}  // namespace

auto attach_component(Registry& registry,
                      Context& context,
                      const Entity definition_entity) -> Entity
{
  TACTILE_ASSERT(is_component_entity(registry, definition_entity));
  TACTILE_ASSERT(!_has_component_type(registry, context, definition_entity));

  const auto attached_component_entity =
      instantiate_component(registry, definition_entity);
  context.comps.push_back(attached_component_entity);

  return attached_component_entity;
}

auto detach_component(Registry& registry,
                      Context& context,
                      const Entity definition_entity) -> Maybe<StringMap<Property>>
{
  TACTILE_ASSERT(is_component_entity(registry, definition_entity));

  const auto iter = _find_component_of_type(registry, context.comps, definition_entity);
  if (iter != context.comps.end()) {
    auto values = registry.get<AttachedComponent>(*iter).attributes;
    context.comps.erase(iter);

    return values;
  }

  return {};
}

auto has_component(const Registry& registry,
                   const Context& context,
                   StringView component_name) -> bool
{
  return std::ranges::any_of(context.comps, [&](const Entity attached_component_entity) {
    const auto& attached_component =
        registry.get<AttachedComponent>(attached_component_entity);
    const auto& component = registry.get<Component>(attached_component.definition);
    return component.name == component_name;
  });
}

}  // namespace tactile::sys
