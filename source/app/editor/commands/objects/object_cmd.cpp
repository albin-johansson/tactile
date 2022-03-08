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

#include "object_cmd.hpp"

#include <utility>  // move

#include "core/components/attribute_context.hpp"
#include "core/systems/object_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

object_cmd::object_cmd(std::string name, registry_ref registry, const object_id id)
    : ACommand{std::move(name)}
    , mRegistry{registry}
    , mObjectId{id}
{}

auto object_cmd::target_object_context() const -> comp::attribute_context&
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_object(registry, mObjectId);
  TACTILE_ASSERT(entity != entt::null);

  return registry.get<comp::attribute_context>(entity);
}

auto object_cmd::target_object() -> comp::object&
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_object(registry, mObjectId);
  TACTILE_ASSERT(entity != entt::null);

  return registry.get<comp::object>(entity);
}

}  // namespace tactile
