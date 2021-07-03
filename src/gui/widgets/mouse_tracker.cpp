#include "mouse_tracker.hpp"

namespace Tactile {
namespace {

inline constexpr auto flags = ImGuiButtonFlags_MouseButtonLeft |
                              ImGuiButtonFlags_MouseButtonMiddle |
                              ImGuiButtonFlags_MouseButtonRight;

}  // namespace

void MouseTracker(const CanvasInfo& canvas, GridState& state)
{
  ImGui::InvisibleButton("MouseTracker", canvas.size, flags);
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
  {
    const auto& io = ImGui::GetIO();
    state.scroll_offset.x += io.MouseDelta.x;
    state.scroll_offset.y += io.MouseDelta.y;
  }
}

}  // namespace Tactile
