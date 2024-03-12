// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <centurion/mouse.hpp>

#include "core/tile/tile_pos.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

/// Provides general information about the mouse, used in response to mouse events.
struct MouseInfo final {
  Float2 pos {};                   ///< The raw mouse position.
  TilePos position_in_viewport;    ///< The hovered tile position.
  cen::mouse_button button {};     ///< The activated mouse button.
  bool is_within_contents : 1 {};  ///< Is the mouse within the tile contents?
};

}  // namespace tactile
