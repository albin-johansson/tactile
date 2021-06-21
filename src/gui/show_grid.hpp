#pragma once

#include "imgui.h"

namespace tactile {

struct GridState final
{
  ImVec2 scroll_offset{};
  ImVec2 grid_size{};
};

struct CanvasInfo final
{
  ImVec2 canvas_size{};
  ImVec2 canvas_screen_pos{};
  ImVec2 canvas_offset_pos{};
};

[[nodiscard]] auto GetCanvasInfo() -> CanvasInfo;

void FillBackground(const CanvasInfo& info);

void ShowGrid(const GridState& state, const CanvasInfo& info);

}  // namespace tactile
