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

#include "core/containers/string.hpp"
#include "core/enums/object_type.hpp"
#include "core/functional/maybe.hpp"
#include "core/math/vector.hpp"
#include "core/prelude.hpp"

namespace tactile {

/// Represents various map objects found in object layers.
/// The current supported object types are rectangles, ellipses, and points.
struct Object final {
  Float2 position {};    ///< Object position.
  Float2 size {};        ///< Object size (might be zero).
  ObjectType type {};    ///< Specific object type.
  String tag;            ///< Optional user-provided tag.
  Maybe<int32> meta_id;  ///< Identifier used in save files.
  bool visible {true};   ///< Is the object rendered?
};

}  // namespace tactile
