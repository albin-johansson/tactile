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

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "common/numeric.hpp"

namespace tactile {

using Int2 = glm::vec<2, int32>;
using Int3 = glm::vec<3, int32>;
using Int4 = glm::vec<4, int32>;

using Float2 = glm::vec2;
using Float3 = glm::vec3;
using Float4 = glm::vec4;

using Vec2i = Int2;
using Vec3i = Int3;
using Vec4i = Int4;

using Vec2 = Float2;
using Vec3 = Float3;
using Vec4 = Float4;

}  // namespace tactile
