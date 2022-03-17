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

#include "alignment.hpp"

#include <imgui_internal.h>

#include "misc/assert.hpp"

namespace tactile {

void center_next_window_on_appearance()
{
  const auto* viewport = ImGui::GetMainViewport();
  const auto pos = viewport->WorkPos;
  const auto size = viewport->WorkSize;

  const ImVec2 next_pos = pos + (size / ImVec2{2, 2});
  const ImVec2 next_pivot{0.5f, 0.5f};

  ImGui::SetNextWindowPos(next_pos, ImGuiCond_Appearing, next_pivot);
}

void center_next_item_horizontally(const float width)
{
  const auto region = ImGui::GetContentRegionAvail();
  const auto halfRegion = region / ImVec2{2.0f, 2.0f};
  ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2{halfRegion.x - (width / 2.0f), 0});
}

void prepare_vertical_alignment_center(const float count)
{
  const auto fontSize = ImGui::GetFontSize();

  const auto region = ImGui::GetContentRegionAvail();
  const auto halfRegion = region / ImVec2{2.0f, 2.0f};

  const auto padding = ImGui::GetStyle().FramePadding;
  const auto height = (count * (fontSize + (2.0f * padding.y)));

  const auto cursorPos = ImGui::GetCursorPos();
  ImGui::SetCursorPos(cursorPos + ImVec2{0, halfRegion.y - (height / 2.0f)});
}

auto right_align_next_item(const float width) -> float
{
  const auto x = ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - width);
  ImGui::SetCursorPosX(x);
  return x;
}

auto right_align_next_item(const char* text) -> float
{
  return right_align_next_item(standard_component_width(text));
}

auto standard_component_width(const char* text) -> float
{
  TACTILE_ASSERT(text);
  return ImGui::CalcTextSize(text).x + (ImGui::GetStyle().FramePadding.x * 2.0f);
}

}  // namespace tactile
