// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <imgui.h>

#include "core/tile/tile_pos.hpp"
#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_STRUCT_NS(tactile, CanvasInfo)

namespace tactile::ui {

struct ViewportCursorInfo final {
  TilePos map_position;
  ImVec2 clamped_position {};
  ImVec2 raw_position {};
  ImVec2 scaled_position {};
  bool is_within_map {};
};

[[nodiscard]] auto get_viewport_cursor_info(const CanvasInfo& canvas_info)
    -> ViewportCursorInfo;

}  // namespace tactile::ui
