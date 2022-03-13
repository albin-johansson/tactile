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

#include "tool_button.hpp"

#include <imgui.h>

#include "editor/gui/common/button.hpp"

namespace tactile {

auto tool_button(const char* label,
                 const char* tooltip,
                 const bool selected,
                 const bool enabled,
                 const float width,
                 const float height) -> bool
{
  if (selected) {
    const auto& style = ImGui::GetStyle();
    ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_HeaderActive]);
  }

  const auto pressed = button(label, tooltip, enabled, width, height);

  if (selected) {
    ImGui::PopStyleColor();
  }

  return pressed;
}

}  // namespace tactile
