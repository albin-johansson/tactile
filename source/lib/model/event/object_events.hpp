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

#include "common/primitives.hpp"
#include "common/type/ecs.hpp"
#include "common/type/math.hpp"
#include "common/type/string.hpp"

namespace tactile {

struct SelectObjectEvent final {
  Entity object_layer {kNullEntity};
  Entity object {kNullEntity};
};

/// Emitted after an object has been moved.
struct MoveObjectEvent final {
  Entity object {kNullEntity};  ///< The object to move.
  Float2 previous;              ///< Previous position of the object.
  Float2 updated;               ///< Updated position of the object.
};

/// Emitted when the visibility of an object should be changed.
struct SetObjectVisibleEvent final {
  Entity object {kNullEntity};  ///< The target object.
  bool visible {};              ///< New visibility state.
};

/// Emitted when the tag of an object should be changed.
struct SetObjectTagEvent final {
  Entity object {kNullEntity};  ///< The target object.
  String tag;                   ///< The new object tag.
};

/// Emitted when the name of an object should be changed.
struct SetObjectNameEvent final {
  Entity object {kNullEntity};  ///< The target object.
  String name;                  ///< The new object name.
};

struct DuplicateObjectEvent final {
  Entity object {kNullEntity};  ///< The target object.
};

struct RemoveObjectEvent final {
  Entity object {kNullEntity};  ///< The target object.
};

struct SpawnObjectContextMenuEvent final {
  Entity object {kNullEntity};
};

}  // namespace tactile