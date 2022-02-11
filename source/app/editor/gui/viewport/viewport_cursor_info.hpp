#pragma once

#include <imgui.h>

#include "core/tile_position.hpp"

namespace tactile {

struct render_info;

struct ViewportCursorInfo final
{
  tile_position map_position;
  ImVec2 clamped_position{};
  ImVec2 raw_position{};
  ImVec2 scaled_position{};
  bool is_within_map{};
};

[[nodiscard]] auto GetViewportCursorInfo(const render_info& info) -> ViewportCursorInfo;

}  // namespace tactile
