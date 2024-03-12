// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "common/type/tile_cache.hpp"
#include "core/tile/tile_pos.hpp"
#include "model/mouse_info.hpp"
#include "model/tool/tool_type.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

struct SelectToolEvent final {
  ToolType type {};
};

struct StampSequenceEvent final {
  UUID layer_id {};
  TileCache old_state;
  TileCache sequence;
};

struct SetStampRandomizerEvent final {
  bool enabled {};  /// Whether the randomizer mode is enabled.
};

struct EraserSequenceEvent final {
  UUID layer_id {};
  TileCache old_state;
};

struct FloodEvent final {
  UUID layer_id {};
  TilePos origin;
  TileID replacement {};
};

/// Emitted when a rectangle should be added to an object layer.
struct AddRectangleEvent final {
  UUID layer_id {};
  Float2 pos {};
  Float2 size {};
};

/// Emitted when an ellipse should be added to an object layer.
struct AddEllipseEvent final {
  UUID layer_id {};
  Float2 pos {};
  Float2 size {};
};

/// Emitted when a point should be added to an object layer.
struct AddPointEvent final {
  UUID layer_id {};
  Float2 pos {};
};

}  // namespace tactile
