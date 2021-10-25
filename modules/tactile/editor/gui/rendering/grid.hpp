#pragma once

#include <imgui.h>

#include "canvas.hpp"
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

/// \addtogroup rendering
/// \{

struct [[deprecated]] GridState final
{
  ImVec2 scroll_offset{};
  ImVec2 grid_size{};
};

[[deprecated]] void ShowGrid(const GridState& state,
                             const CanvasInfo& canvas,
                             uint32 lineColor = IM_COL32(200, 200, 200, 40));

/// \} End of group rendering

}  // namespace Tactile
