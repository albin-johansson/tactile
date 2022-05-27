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

#include "actor.hpp"

#include "misc/panic.hpp"

namespace tactile {

Actor::Actor(entt::registry& registry, entt::entity entity)
    : mRegistry{registry}
    , mEntity{entity}
{
  if (!registry.valid(mEntity)) {
    throw TactileError{"Invalid entity identifier!"};
  }
}

auto Actor::of(entt::registry& registry) -> Actor
{
  return Actor{registry, registry.create()};
}

}  // namespace tactile