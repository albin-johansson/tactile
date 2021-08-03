#pragma once

#include <imgui.h>

#include "core/map/map_position.hpp"

namespace Tactile {

struct RenderInfo;

struct ViewportCursorInfo final
{
  MapPosition map_position;
  ImVec2 clamped_position{};
  ImVec2 raw_position{};
  bool is_within_map{};
};

[[nodiscard]] auto GetViewportCursorInfo(const RenderInfo& info)
    -> ViewportCursorInfo;

}  // namespace Tactile
