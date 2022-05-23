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

#pragma once

#include <entt/entity/registry.hpp>

#include "misc/panic.hpp"

namespace tactile {

/**
 * \brief Returns a component featured by an entity.
 *
 * \tparam T the type of the component.
 *
 * \param registry the source registry.
 * \param entity the entity to query.
 *
 * \return a reference to the component.
 *
 * \throws TactileError if the entity identifier is invalid or if the entity does not
 * have the component.
 */
template <typename T>
[[nodiscard]] auto checked_get(entt::registry& registry, const entt::entity entity) -> T&
{
  if (!registry.valid(entity)) {
    throw TactileError{"Invalid entity identifier!"};
  }

  if (auto* comp = registry.try_get<T>(entity)) {
    return *comp;
  }
  else {
    throw TactileError{"Entity did not feature requested component!"};
  }
}

template <typename T>
[[nodiscard]] auto checked_get(const entt::registry& registry, const entt::entity entity)
    -> const T&
{
  if (!registry.valid(entity)) {
    throw TactileError{"Invalid entity identifier!"};
  }

  if (const auto* comp = registry.try_get<T>(entity)) {
    return *comp;
  }
  else {
    throw TactileError{"Entity did not feature requested component!"};
  }
}

template <typename T>
[[nodiscard]] auto ctx_get(entt::registry& registry) -> T&
{
  if (auto* comp = registry.ctx().find<T>()) {
    return *comp;
  }
  else {
    throw TactileError{"Context did not feature requested component!"};
  }
}

template <typename T>
[[nodiscard]] auto ctx_get(const entt::registry& registry) -> const T&
{
  if (const auto* comp = registry.ctx().find<T>()) {
    return *comp;
  }
  else {
    throw TactileError{"Context did not feature requested component!"};
  }
}

}  // namespace tactile
