// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/entity/entity.hpp>

#include "model/mouse_info.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

struct ViewportMousePressedEvent final {
  MouseInfo info;
};

struct ViewportMouseDraggedEvent final {
  MouseInfo info;
};

struct ViewportMouseReleasedEvent final {
  MouseInfo info;
};

struct ViewportMouseEnteredEvent final {};

struct ViewportMouseExitedEvent final {};

struct CenterViewportEvent final {};

struct ResetZoomEvent final {};

struct DecreaseZoomEvent final {};

struct IncreaseZoomEvent final {};

struct OffsetDocumentViewportEvent final {
  Float2 delta {};  /// The offset to apply.
};

struct OffsetTilesetViewportEvent final {
  UUID tileset_id {};  /// The target tileset.
  Float2 delta {};     /// The offset to apply.
};

/// Event used to update the limits of tilesets in the tileset dock widget.
struct UpdateTilesetViewportLimitsEvent final {
  UUID tileset_id {};    /// The target tileset.
  Float2 min_offset {};  /// The minimum offset.
  Float2 max_offset {};  /// The maximum offset.
};

struct PanUpEvent final {};

struct PanDownEvent final {};

struct PanLeftEvent final {};

struct PanRightEvent final {};

}  // namespace tactile
