#include "style.hpp"

#include "imgui.h"

namespace Tactile {

void ApplyStyle(ImGuiStyle& style)
{
  style.WindowBorderSize = 0;

  style.WindowPadding = ImVec2{10, 10};
  style.FramePadding = ImVec2{5, 5};
  style.CellPadding = ImVec2{4, 4};

  style.ItemSpacing = ImVec2{8, 8};
  style.ItemInnerSpacing = ImVec2{8, 6};
  style.IndentSpacing = 15;

  style.GrabMinSize = 6;
  style.ScrollbarSize = 10;

  style.WindowRounding = 4;
  style.FrameRounding = 4;
  style.PopupRounding = 4;
  style.ScrollbarRounding = 4;
  style.GrabRounding = 4;
  style.TabRounding = 4;
}

}  // namespace Tactile
