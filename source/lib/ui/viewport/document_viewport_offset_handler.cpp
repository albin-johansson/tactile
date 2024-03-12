// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "document_viewport_offset_handler.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "model/event/viewport_events.hpp"

namespace tactile::ui {

void update_document_viewport_offset(const ImVec2& viewport_size,
                                     entt::dispatcher& dispatcher)
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
      dispatcher.enqueue<OffsetDocumentViewportEvent>(delta);
    }
  }
}

}  // namespace tactile::ui
