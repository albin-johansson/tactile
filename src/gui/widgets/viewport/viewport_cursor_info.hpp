#pragma once

#include <imgui.h>

#include "core/map/map_position.hpp"

namespace Tactile {

struct ViewportCursorInfo final
{
  MapPosition map_position;
  ImVec2 raw_position{};
  bool is_within_map{};
};

[[nodiscard]] auto GetViewportCursorInfo(const ImVec2& mapOrigin,
                                         const ImVec2& gridSize,
                                         float nRows,
                                         float nCols) -> ViewportCursorInfo;

}  // namespace Tactile
