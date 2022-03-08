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

#include "object_system.hpp"

#include "core/components/object.hpp"
#include "misc/throw.hpp"

namespace tactile::sys {

auto find_object(const entt::registry& registry, const object_id id) -> entt::entity
{
  for (auto&& [entity, object] : registry.view<comp::object>().each()) {
    if (object.id == id) {
      return entity;
    }
  }

  return entt::null;
}

auto get_object(const entt::registry& registry, const object_id id) -> entt::entity
{
  const auto entity = find_object(registry, id);
  if (entity != entt::null) {
    return entity;
  }
  else {
    throw_traced(tactile_error{"Invalid object identifier!"});
  }
}

}  // namespace tactile::sys
