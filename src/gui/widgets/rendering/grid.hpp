#pragma once

#include <imgui.h>

#include "aliases/ints.hpp"
#include "gui/widgets/rendering/canvas.hpp"

namespace Tactile {

struct [[deprecated]] GridState final
{
  ImVec2 scroll_offset{};
  ImVec2 grid_size{};
};

[[deprecated]] void ShowGrid(const GridState& state,
                             const CanvasInfo& canvas,
                             uint32 lineColor = IM_COL32(200, 200, 200, 40));

}  // namespace Tactile
