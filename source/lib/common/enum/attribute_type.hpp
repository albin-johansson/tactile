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

namespace tactile {

/// Represents the different possible attributes types.
enum class AttributeType {
  String,  ///< Arbitrary string.
  Int,     ///< Plain int.
  Int2,    ///< 2D int vector.
  Int3,    ///< 3D int vector.
  Int4,    ///< 4D int vector.
  Float,   ///< Plain float.
  Float2,  ///< 2D float vector.
  Float3,  ///< 3D float vector.
  Float4,  ///< 4D float vector.
  Bool,    ///< Boolean value.
  Path,    ///< File path.
  Color,   ///< Color value.
  Object,  ///< Integer object ID, references some map object.
};

}  // namespace tactile