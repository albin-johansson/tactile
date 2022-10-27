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

#include "render.hpp"

#include <centurion/color.hpp>
#include <imgui_internal.h>

#include "editor/ui/conversions.hpp"
#include "editor/ui/textures.hpp"

namespace tactile::ui {
namespace {

[[nodiscard]] constexpr auto to_u32(const cen::color& color) -> uint32
{
  return IM_COL32(color.red(), color.green(), color.blue(), color.alpha());
}

}  // namespace

void draw_rect(const ImVec2& position,
               const ImVec2& size,
               const uint32 color,
               const float thickness)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddRect(position, position + size, color, 0, 0, thickness);
}

void draw_rect(const Float2& position,
               const Float2& size,
               const cen::color& color,
               const float thickness)
{
  draw_rect(from_vec(position), from_vec(size), to_u32(color), thickness);
}

void fill_rect(const ImVec2& position, const ImVec2& size, const uint32 color)
{
  auto* list = ImGui::GetWindowDrawList();
  list->AddRectFilled(position, position + size, color);
}

void fill_rect(const Float2& position, const Float2& size, const cen::color& color)
{
  fill_rect(from_vec(position), from_vec(size), to_u32(color));
}

}  // namespace tactile::ui
