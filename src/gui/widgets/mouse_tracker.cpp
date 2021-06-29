#include "mouse_tracker.hpp"

namespace Tactile {
namespace {

inline constexpr auto flags =
    ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight;

}  // namespace

void MouseTracker(const CanvasInfo& info, GridState& state)
{
  ImGui::InvisibleButton("MouseTracker", info.canvas_size, flags);
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
  {
    const auto& io = ImGui::GetIO();
    state.scroll_offset.x += io.MouseDelta.x;
    state.scroll_offset.y += io.MouseDelta.y;
  }
}

}  // namespace Tactile
