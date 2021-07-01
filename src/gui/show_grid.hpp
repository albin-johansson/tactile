#pragma once

#include "aliases/ints.hpp"
#include "canvas_info.hpp"
#include "imgui.h"

namespace Tactile {

struct GridState final
{
  ImVec2 scroll_offset{};
  ImVec2 grid_size{};
};

void FillBackground(const CanvasInfo& canvas);

void ShowGrid(const GridState& state,
              const CanvasInfo& canvas,
              uint32 lineColor = IM_COL32(200, 200, 200, 40));

}  // namespace Tactile
