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

/// Represents different attribute value types.
enum class AttributeType {
  String,  ///< An arbitrary string.
  Int,     ///< A signed integer.
  Int2,    ///< A two-dimensional vector of integers.
  Int3,    ///< A three-dimensional vector of integers.
  Int4,    ///< A four-dimensional vector of integers.
  Float,   ///< A floating-point number.
  Float2,  ///< A two-dimensional vector of floats.
  Float3,  ///< A three-dimensional vector of floats.
  Float4,  ///< A four-dimensional vector of floats.
  Bool,    ///< A Boolean value.
  Path,    ///< A file path.
  Color,   ///< A color value.
  Object,  ///< An integer ID that references an object in a map.
};

}  // namespace tactile
