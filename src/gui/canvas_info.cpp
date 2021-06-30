#include "canvas_info.hpp"

#include "imgui_internal.h"

namespace Tactile {

auto GetCanvasInfo() -> CanvasInfo
{
  CanvasInfo info;

  info.size = ImGui::GetContentRegionAvail();
  info.tl = ImGui::GetCursorScreenPos();
  info.br = info.tl + info.size;

  return info;
}

}  // namespace Tactile
