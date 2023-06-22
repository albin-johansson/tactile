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

[[nodiscard]] auto _find_component_of_type(const Model& model,
                                           const Vector<Entity>& component_entities,
                                           const Entity definition_entity)
    -> Vector<Entity>::const_iterator
{
  return std::ranges::find_if(component_entities, [&](const Entity component_entity) {
    const auto& component = model.get<Component>(component_entity);
    return component.definition == definition_entity;
  });
}

[[nodiscard]] auto _has_component_type(const Model& model,
                                       const Context& context,
                                       const Entity definition_entity) -> bool
{
  const auto iter = _find_component_of_type(model, context.comps, definition_entity);
  return iter != context.comps.end();
}

}  // namespace

auto attach_component(Model& model, Context& context, const Entity definition_entity)
    -> Entity
{
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));
  TACTILE_ASSERT(!_has_component_type(model, context, definition_entity));

  const auto component_entity = instantiate_component(model, definition_entity);
  context.comps.push_back(component_entity);

  return component_entity;
}

auto detach_component(Model& model, Context& context, const Entity definition_entity)
    -> Maybe<StringMap<Attribute>>
{
  TACTILE_ASSERT(is_component_definition_entity(model, definition_entity));

  const auto iter = _find_component_of_type(model, context.comps, definition_entity);
  if (iter != context.comps.end()) {
    auto values = model.get<Component>(*iter).attributes;
    context.comps.erase(iter);

    return values;
  }

  return nothing;
}

auto has_component(const Model& model, const Context& context, StringView component_name)
    -> bool
{
  return std::ranges::any_of(context.comps, [&](const Entity component_entity) {
    const auto& component = model.get<Component>(component_entity);
    const auto& definition = model.get<ComponentDefinition>(component.definition);
    return definition.name == component_name;
  });
}

}  // namespace tactile::sys
