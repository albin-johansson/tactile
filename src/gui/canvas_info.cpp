#include "canvas_info.hpp"

#include "imgui_internal.h"

namespace Tactile {

auto GetCanvasInfo() -> CanvasInfo
{
  CanvasInfo info;

  info.canvas_size = ImGui::GetContentRegionAvail();
  info.canvas_tl = ImGui::GetCursorScreenPos();
  info.canvas_br = info.canvas_tl + info.canvas_size;

  return info;
}

}  // namespace Tactile
