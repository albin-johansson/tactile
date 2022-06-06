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

#include <string>  // string

#include <entt/entity/entity.hpp>

#include "core/common/identifiers.hpp"
#include "core/common/math.hpp"
#include "core/object_type.hpp"

namespace tactile::comp {

/// Component that represents objects located in object layers.
struct Object final
{
  ObjectID id{};      ///< Unique object identifier.
  Vector2f pos{};     ///< Object position.
  Vector2f size{};    ///< Object size (might be zero).
  ObjectType type{};  ///< Specific object type.
  std::string tag;    ///< Optional user-provided type.
  bool visible{};     ///< Whether or not the object is rendered.
};

/// Component that provides temporary information about dragged objects.
struct ObjectDragInfo final
{
  Vector2f origin_object_pos{};  ///< Object position at drag start.
  Vector2f last_mouse_pos{};     ///< Mouse viewport position at last update.
};

}  // namespace tactile::comp
