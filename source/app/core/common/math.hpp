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

#include <concepts>  // unsigned_integral

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace tactile {

using Vector2f = glm::vec2;
using Vector2i = glm::ivec2;
using Vector4f = glm::vec4;
using Vector4i = glm::ivec4;

/// Returns the difference between two unsigned integers
template <std::unsigned_integral T>
[[nodiscard]] constexpr auto udiff(const T a, const T b) noexcept -> T
{
  return (a < b) ? (b - a) : (a - b);
}

}  // namespace tactile
