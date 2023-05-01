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

#include "context_system.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "common/util/assoc.hpp"
#include "model/systems/component_system.hpp"

namespace tactile::sys {

auto is_context_entity(const Model& model, const Entity entity) -> bool
{
  return entity != kNullEntity && model.has<Context>(entity);
}

void rename_property(Model& model,
                     const Entity context_entity,
                     StringView current_name,
                     String new_name)
{
  TACTILE_ASSERT(is_context_entity(model, context_entity));

  auto& context = model.get<Context>(context_entity);
  TACTILE_ASSERT(!context.props.contains(new_name));

  if (const auto iter = context.props.find(current_name); iter != context.props.end()) {
    auto property_value = iter->second;
    context.props.erase(iter);
    context.props[new_name] = std::move(property_value);
  }
}

auto copy_context_component_from(Model& model, const Entity context_entity) -> Context
{
  TACTILE_ASSERT(is_context_entity(model, context_entity));
  const auto& src_context = model.get<Context>(context_entity);

  Context new_context;
  new_context.name = src_context.name;
  new_context.props = src_context.props;

  new_context.comps.reserve(src_context.comps.size());
  for (const auto src_component_entity: src_context.comps) {
    new_context.comps.push_back(copy_component(model, src_component_entity));
  }

  return new_context;
}

}  // namespace tactile::sys
