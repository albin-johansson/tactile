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

#include "document_viewport_offset_handler.hpp"

#include <imgui_internal.h>

#include "model/events/viewport_events.hpp"

namespace tactile::ui {

void update_document_viewport_offset(const Entity viewport_entity,
                                     const ImVec2& viewport_size,
                                     Dispatcher& dispatcher)
{
  if (ImGui::GetTopMostPopupModal() != nullptr) {
    return;
  }

  ImGui::InvisibleButton("update_document_viewport_offset",
                         viewport_size,
                         ImGuiButtonFlags_MouseButtonLeft |
                             ImGuiButtonFlags_MouseButtonMiddle |
                             ImGuiButtonFlags_MouseButtonRight);

  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
    const auto& io = ImGui::GetIO();
    const Float2 delta {io.MouseDelta.x, io.MouseDelta.y};
    if (delta != Float2 {0, 0}) {
      dispatcher.enqueue<OffsetViewportEvent>(viewport_entity, delta);
    }
  }
}

}  // namespace tactile::ui
