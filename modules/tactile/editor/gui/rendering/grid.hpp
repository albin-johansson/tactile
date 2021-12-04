#pragma once

#include <tactile_def.hpp>

#include <imgui.h>

namespace Tactile {

struct CanvasInfo;

/// \addtogroup rendering
/// \{

void RenderGrid(const ImVec2& offset,
                const ImVec2& gridSize,
                const CanvasInfo& canvas,
                uint32 lineColor = IM_COL32(200, 200, 200, 40));

/// \} End of group rendering

}  // namespace Tactile
