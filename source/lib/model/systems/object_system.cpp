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

#include "common/debug/assert.hpp"
#include "components/context.hpp"
#include "components/object.hpp"
#include "model/systems/context/context_system.hpp"
#include "model/systems/validation_system.hpp"

namespace tactile::sys {

auto create_object(Model& model, const ObjectType type) -> Entity
{
  const auto object_entity = model.create_entity();

  auto& object = model.add<Object>(object_entity);
  object.type = type;

  auto& context = model.add<Context>(object_entity);
  context.name = "Object";

  TACTILE_ASSERT(is_object_entity(model, object_entity));
  return object_entity;
}

auto duplicate_object(Model& model, const Entity object_entity) -> Entity
{
  TACTILE_ASSERT(is_object_entity(model, object_entity));
  const auto& src_context = model.get<Context>(object_entity);
  const auto& src_object = model.get<Object>(object_entity);

  const auto new_object_entity = model.create_entity();

  auto& new_object = model.add<Object>(new_object_entity);
  new_object = src_object;
  new_object.meta_id.reset();

  auto& new_context = model.add<Context>(new_object_entity);
  new_context = copy_context(model, src_context);

  TACTILE_ASSERT(is_object_entity(model, new_object_entity));
  return new_object_entity;
}

}  // namespace tactile::sys
