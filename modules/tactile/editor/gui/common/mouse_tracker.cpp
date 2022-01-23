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
