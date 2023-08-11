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

#include <glm/gtx/io.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "tactile/core/common/prelude.hpp"

namespace tactile {

using Int2 = glm::ivec2;
using Int3 = glm::ivec3;
using Int4 = glm::ivec4;

using Float2 = glm::vec2;
using Float3 = glm::vec3;
using Float4 = glm::vec4;

inline constexpr uint32 kVectorXBit = 1u << 0u;
inline constexpr uint32 kVectorYBit = 1u << 1u;
inline constexpr uint32 kVectorZBit = 1u << 2u;
inline constexpr uint32 kVectorWBit = 1u << 3u;

/** Returns a bitmask of components that are equal in both vectors. */
template <typename Vec>
[[nodiscard]] auto compare_vector_components(const Vec& a, const Vec& b) -> uint32
{
  uint32 mask = 0;

  if (a.x == b.x) {
    mask |= kVectorXBit;
  }

  if (a.y == b.y) {
    mask |= kVectorYBit;
  }

  const int dimensions = a.length();

  if (dimensions >= 3 && a[2] == b[2]) {
    mask |= kVectorZBit;
  }

  if (dimensions >= 4 && a[3] == b[3]) {
    mask |= kVectorWBit;
  }

  return mask;
}

}  // namespace tactile