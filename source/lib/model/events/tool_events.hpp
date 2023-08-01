/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/enum/tool_type.hpp"
#include "common/tile_pos.hpp"
#include "common/type/ecs.hpp"
#include "common/type/math.hpp"
#include "common/type/tile_cache.hpp"
#include "core/prelude.hpp"
#include "model/viewports/viewport_components.hpp"

namespace tactile {

struct SelectToolEvent final {
  ToolType type {};
};

struct StampSequenceEvent final {
  Entity layer {kNullEntity};
  TileCache old_state;
  TileCache sequence;
};

struct SetStampRandomizerEvent final {
  bool enabled {};  ///< Whether the randomizer mode is enabled.
};

struct EraserSequenceEvent final {
  Entity layer {kNullEntity};
  TileCache old_state;
};

struct FloodEvent final {
  Entity layer {kNullEntity};
  TilePos origin;
  TileID replacement {};
};

/// Emitted when a rectangle should be added to an object layer.
struct AddRectangleEvent final {
  Entity layer {kNullEntity};
  Float2 pos {};
  Float2 size {};
};

/// Emitted when an ellipse should be added to an object layer.
struct AddEllipseEvent final {
  Entity layer {kNullEntity};
  Float2 pos {};
  Float2 size {};
};

/// Emitted when a point should be added to an object layer.
struct AddPointEvent final {
  Entity layer {kNullEntity};
  Float2 pos {};
};

}  // namespace tactile