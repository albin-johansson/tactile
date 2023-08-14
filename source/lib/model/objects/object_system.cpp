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

#include "object_system.hpp"

#include "model/contexts/context_components.hpp"
#include "model/contexts/context_system.hpp"
#include "model/entity_validation.hpp"
#include "model/objects/object_components.hpp"
#include "tactile/core/common/assert.hpp"

namespace tactile::sys {

auto duplicate_object(Registry& registry, const Entity object_entity) -> Entity
{
  TACTILE_ASSERT(is_object_entity(registry, object_entity));
  const auto& src_context = registry.get<Context>(object_entity);
  const auto& src_object = registry.get<Object>(object_entity);

  const auto new_object_entity = registry.create_entity();

  auto& new_object = registry.add<Object>(new_object_entity);
  new_object = src_object;
  new_object.meta_id.reset();

  auto& new_context = registry.add<Context>(new_object_entity);
  new_context = copy_context(registry, src_context);

  TACTILE_ASSERT(is_object_entity(registry, new_object_entity));
  return new_object_entity;
}

}  // namespace tactile::sys
