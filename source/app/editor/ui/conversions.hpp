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

#include <imgui.h>

#include "core/common/math.hpp"
#include "core/tile_pos.hpp"

namespace tactile::ui {

[[nodiscard]] inline auto from_pos(const TilePos& pos) noexcept -> ImVec2
{
  return {static_cast<float>(pos.col()), static_cast<float>(pos.row())};
}

[[nodiscard]] inline auto from_vec(const Vector2f& vec) noexcept -> ImVec2
{
  return {vec.x, vec.y};
}

[[nodiscard]] inline auto from_vec(const Vector2i& vec) noexcept -> ImVec2
{
  return {static_cast<float>(vec.x), static_cast<float>(vec.y)};
}

[[nodiscard]] inline auto from_vec(const Vector4f& vec) noexcept -> ImVec4
{
  return {vec.x, vec.y, vec.z, vec.w};
}

[[nodiscard]] inline auto from_vec(const Vector4i& vec) noexcept -> ImVec4
{
  return {static_cast<float>(vec.x),
          static_cast<float>(vec.y),
          static_cast<float>(vec.z),
          static_cast<float>(vec.w)};
}

}  // namespace tactile::ui
