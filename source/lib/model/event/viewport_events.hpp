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

#include "common/type/ecs.hpp"
#include "common/type/math.hpp"
#include "model/viewports/viewport_components.hpp"

namespace tactile {

struct ViewportMousePressedEvent final {
  ViewportMouseInfo mouse_info;
};

struct ViewportMouseDraggedEvent final {
  ViewportMouseInfo mouse_info;
};

struct ViewportMouseReleasedEvent final {
  ViewportMouseInfo mouse_info;
};

struct ViewportMouseEnteredEvent final {};

struct ViewportMouseExitedEvent final {};

/// Event for centering a viewport over its contents.
struct CenterViewportEvent final {
  Entity viewport {kNullEntity};  ///< The target viewport entity.
};

/// Event for resetting the size of tiles in a viewport.
struct ResetViewportZoomEvent final {
  Entity viewport {kNullEntity};  ///< The target viewport entity.
};

/// Event for increasing the tile size in a viewport.
struct IncreaseViewportZoomEvent final {
  Entity viewport {kNullEntity};  ///< The target viewport entity.
  Float2 anchor_pos {};           ///< The anchor location of the zoom.
};

/// Event for decreasing the tile size in a viewport.
struct DecreaseViewportZoomEvent final {
  Entity viewport {kNullEntity};  ///< The target viewport entity.
  Float2 anchor_pos {};           ///< The anchor location of the zoom.
};

/// Event for nudging the origin in a viewport.
struct OffsetViewportEvent final {
  Entity viewport {kNullEntity};  ///< The target viewport entity.
  Float2 delta {};                ///< The offset to apply.
};

/// Event for setting the offset limits of a viewport.
struct SetViewportLimitsEvent final {
  Entity viewport {kNullEntity};  ///< The target viewport entity.
  Float2 min_offset {};           ///< The new minimum offset.
  Float2 max_offset {};           ///< The new maximum offset.
};

struct SetDynamicViewportInfoEvent final {
  Entity viewport {kNullEntity};  ///< The target viewport entity.
  DynamicViewportInfo info;
};

/// Event for moving a viewport "up".
struct PanViewportUpEvent final {
  Entity viewport {kNullEntity};  ///< The target viewport entity.
};

/// Event for moving a viewport "down".
struct PanViewportDownEvent final {
  Entity viewport {kNullEntity};  ///< The target viewport entity.
};

/// Event for moving a viewport "left".
struct PanViewportLeftEvent final {
  Entity viewport {kNullEntity};  ///< The target viewport entity.
};

/// Event for moving a viewport "right".
struct PanViewportRightEvent final {
  Entity viewport {kNullEntity};  ///< The target viewport entity.
};

}  // namespace tactile