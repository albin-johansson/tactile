#include "mouse_tracker.hpp"

#include "editor/events/viewport_events.hpp"
#include "editor/gui/rendering/canvas.hpp"

namespace Tactile {
namespace {

constexpr auto gButtonFlags = ImGuiButtonFlags_MouseButtonLeft |
                              ImGuiButtonFlags_MouseButtonMiddle |
                              ImGuiButtonFlags_MouseButtonRight;

}  // namespace

auto TrackScrollOffset(const CanvasInfo& canvas, ImVec2 scrollOffset) -> ImVec2
{
  ImGui::InvisibleButton("TrackScrollOffset", canvas.size, gButtonFlags);
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
  ImGui::InvisibleButton("UpdateViewportOffset", canvas.size, gButtonFlags);
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
    const auto& io = ImGui::GetIO();
    dispatcher.enqueue<OffsetViewportEvent>(io.MouseDelta.x, io.MouseDelta.y);
  }
}

}  // namespace Tactile
