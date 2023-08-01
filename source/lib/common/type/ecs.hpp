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

#pragma once

#include <entt/entity/entity.hpp>

#include "core/functional/func.hpp"

namespace tactile {

using Entity = entt::entity;

inline constexpr Entity kNullEntity = entt::null;

/// Tag component attached to entities that should not be used.
/// This is commonly used by commands and similar facilities to prevent "hidden" entities
/// from being mistaken as real entities. For example, a layer entity associated with a
/// layer that is removed from a map will be marked as disabled using this tag type by an
/// associated command.
struct DisabledTag final {};

using EntityCallback = Func<void(Entity)>;

}  // namespace tactile
