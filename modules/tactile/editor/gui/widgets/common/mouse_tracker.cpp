#include "mouse_tracker.hpp"

#include "tactile/editor/events/viewport_events.hpp"
#include "tactile/editor/gui/rendering/canvas.hpp"

namespace Tactile {
namespace {

constexpr auto button_flags = ImGuiButtonFlags_MouseButtonLeft |
                              ImGuiButtonFlags_MouseButtonMiddle |
                              ImGuiButtonFlags_MouseButtonRight;

}  // namespace

auto TrackScrollOffset(const CanvasInfo& canvas, ImVec2 scrollOffset) -> ImVec2
{
  ImGui::InvisibleButton("TrackScrollOffset", canvas.size, button_flags);
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
    const auto& io = ImGui::GetIO();
    scrollOffset.x += io.MouseDelta.x;
    scrollOffset.y += io.MouseDelta.y;
    return scrollOffset;
  }
  else {
    return scrollOffset;
  }
}

void UpdateViewportOffset(const CanvasInfo& canvas, entt::dispatcher& dispatcher)
{
  ImGui::InvisibleButton("UpdateViewportOffset", canvas.size, button_flags);
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
    const auto& io = ImGui::GetIO();
    dispatcher.enqueue<OffsetViewportEvent>(io.MouseDelta.x, io.MouseDelta.y);
  }
}

}  // namespace Tactile
