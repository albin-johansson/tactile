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
#include "core/common/uuid.hpp"

namespace tactile {

/// Emitted after an object has been moved.
struct MoveObjectEvent final
{
  UUID     object_id {};  /// ID of the object to move.
  Vector2f previous;      /// Previous position of the object.
  Vector2f updated;       /// Updated position of the object.
};

/// Emitted when the visibility of an object should be changed.
struct SetObjectVisibleEvent final
{
  UUID object_id {};  /// ID of the target object.
  bool visible {};    /// New visibility state.
};

/// Emitted when the tag of an object should be changed.
struct SetObjectTagEvent final
{
  UUID        object_id {};  /// ID of target object.
  std::string tag;           /// The new object tag.
};

/// Emitted when the name of an object should be changed.
struct SetObjectNameEvent final
{
  UUID        object_id {};  /// ID of target object.
  std::string name;          /// The new object name.
};

struct DuplicateObjectEvent final
{
  UUID object_id {};  /// ID of the target object.
};

struct RemoveObjectEvent final
{
  UUID object_id {};  /// ID of the target object.
};

struct SpawnObjectContextMenuEvent final
{
  UUID object_id {};
};

}  // namespace tactile