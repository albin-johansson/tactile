#include "canvas.hpp"

#include <imgui_internal.h>

#include <tactile-base/tactile_std.hpp>

#include "core/utils/color_utils.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

constexpr uint32 border_color = IM_COL32(255, 255, 255, 100);

}  // namespace

void ClearBackground(const CanvasInfo& canvas)
{
  auto* drawList = ImGui::GetWindowDrawList();
  drawList->AddRectFilled(canvas.tl,
                          canvas.br,
                          ColorToU32(Prefs::GetViewportBackground()));
  drawList->AddRect(canvas.tl, canvas.br, border_color);
}

auto GetCanvasInfo() -> CanvasInfo
{
  CanvasInfo canvas;

  canvas.size = ImGui::GetContentRegionAvail();
  canvas.tl = ImGui::GetCursorScreenPos();
  canvas.br = canvas.tl + canvas.size;

  return canvas;
}

}  // namespace Tactile
