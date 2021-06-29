#pragma once

#include "imgui.h"
#include "canvas_info.hpp"

namespace tactile {

struct GridState final
{
  ImVec2 scroll_offset{};
  ImVec2 grid_size{};
};

void FillBackground(const CanvasInfo& info);

void ShowGrid(const GridState& state, const CanvasInfo& info);

}  // namespace tactile
