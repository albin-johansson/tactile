#pragma once

#include <imgui.h>

#include "core/tile_position.hpp"

namespace tactile {

struct RenderInfo;

struct ViewportCursorInfo final
{
  tile_position map_position;
  ImVec2 clamped_position{};
  ImVec2 raw_position{};
  bool is_within_map{};
};

[[nodiscard]] auto GetViewportCursorInfo(const RenderInfo& info) -> ViewportCursorInfo;

}  // namespace tactile
