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

#include "labels.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "misc/assert.hpp"

namespace tactile::ui {

void centered_label(const char* text)
{
  TACTILE_ASSERT(text);
  const auto textSize = ImGui::CalcTextSize(text);

  const auto region = ImGui::GetContentRegionAvail();
  const auto halfRegion = region / ImVec2{2.0f, 2.0f};

  const auto cursorPos = ImGui::GetCursorPos();
  ImGui::SetCursorPos(cursorPos + ImVec2{halfRegion.x - (textSize.x / 2.0f), 0});
  ImGui::TextUnformatted(text);
}

}  // namespace tactile::ui
