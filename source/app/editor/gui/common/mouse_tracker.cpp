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

#include "mouse_tracker.hpp"

#include <imgui.h>

#include "editor/events/viewport_events.hpp"

namespace tactile {
namespace {

constexpr auto gButtonFlags = ImGuiButtonFlags_MouseButtonLeft |
                              ImGuiButtonFlags_MouseButtonMiddle |
                              ImGuiButtonFlags_MouseButtonRight;

}  // namespace

void UpdateViewportOffset(const ImVec2& viewportSize, entt::dispatcher& dispatcher)
{
  ImGui::InvisibleButton("UpdateViewportOffset", viewportSize, gButtonFlags);
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
    const auto& io = ImGui::GetIO();
    dispatcher.enqueue<OffsetViewportEvent>(io.MouseDelta.x, io.MouseDelta.y);
  }
}

}  // namespace tactile
