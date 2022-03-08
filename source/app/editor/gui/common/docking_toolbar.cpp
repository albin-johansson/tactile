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

#include "docking_toolbar.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "misc/assert.hpp"

namespace tactile {

/**
 * This widget is shamelessly based on an example provided in this issue:
 * https://github.com/ocornut/imgui/issues/2648.
 */

void begin_docking_toolbar(const char* name, int& toolbarAxis)
{
  TACTILE_ASSERT(name);

  const auto requestedSize =
      (toolbarAxis == ImGuiAxis_X) ? ImVec2{-1.0f, 0.0f} : ImVec2{0.0f, -1.0f};
  ImGui::SetNextWindowSize(requestedSize);

  ImGuiWindowClass windowClass;
  windowClass.DockingAllowUnclassed = true;
  windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoCloseButton;
  windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_HiddenTabBar;
  windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingSplitMe;
  windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverMe;
  windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverOther;
  if (toolbarAxis == ImGuiAxis_X) {
    windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeY;
  }
  else {
    windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeX;
  }
  ImGui::SetNextWindowClass(&windowClass);

  const auto fontSize = ImGui::GetFontSize();
  const ImVec2 iconSize{ImFloor(fontSize * 1.7f), ImFloor(fontSize * 1.7f)};

  constexpr auto window_flags = ImGuiWindowFlags_NoCollapse |
                                ImGuiWindowFlags_NoTitleBar |
                                ImGuiWindowFlags_NoScrollbar;
  ImGui::Begin(name, nullptr, window_flags);

  if (auto* node = ImGui::GetWindowDockNode()) {
    auto& style = ImGui::GetStyle();

    assert((toolbarAxis ^ 1) >= 0);
    const auto toolbarAxisPerp = static_cast<usize>(toolbarAxis ^ 1);
    const auto windowPadding = style.WindowPadding[toolbarAxisPerp];
    const auto sizeWhenDocked = windowPadding * 2.0f + iconSize[toolbarAxisPerp];

    node->WantLockSizeOnce = true;
    node->SizeRef[toolbarAxisPerp] = sizeWhenDocked;
    node->Size[toolbarAxisPerp] = sizeWhenDocked;

    if (node->ParentNode && node->ParentNode->SplitAxis != ImGuiAxis_None) {
      toolbarAxis = static_cast<ImGuiAxis>(node->ParentNode->SplitAxis ^ 1);
    }
  }
}

void end_docking_toolbar()
{
  ImGui::End();
}

}  // namespace tactile
