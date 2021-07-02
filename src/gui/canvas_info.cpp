#include "canvas_info.hpp"

#include <imgui_internal.h>

namespace Tactile {

auto GetCanvasInfo() -> CanvasInfo
{
  CanvasInfo canvas;

  canvas.size = ImGui::GetContentRegionAvail();
  canvas.tl = ImGui::GetCursorScreenPos();
  canvas.br = canvas.tl + canvas.size;

  return canvas;
}

}  // namespace Tactile
