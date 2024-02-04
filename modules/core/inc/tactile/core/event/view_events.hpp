// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * An event for centering a viewport over a document.
 */
struct CenterViewportEvent final {
  UUID document_uuid;
};

/**
 * An event for increasing the zoom of a document viewport.
 */
struct IncreaseViewportZoomEvent final {
  UUID document_uuid;
};

/**
 * An event for decreasing the zoom of a document viewport.
 */
struct DecreaseViewportZoomEvent final {
  UUID document_uuid;
};

/**
 * An event for resetting the zoom of a document viewport.
 */
struct ResetViewportZoomEvent final {
  UUID document_uuid;
};

/**
 * An event for toggling document viewport grid rendering.
 */
struct ToggleViewportGridEvent final {
  UUID document_uuid;
};

/**
 * An event for panning a document viewport upward.
 */
struct PanViewportUpEvent final {
  UUID document_uuid;
};

/**
 * An event for panning a document viewport downward.
 */
struct PanViewportDownEvent final {
  UUID document_uuid;
};

/**
 * An event for panning a document viewport leftward.
 */
struct PanViewportLeftEvent final {
  UUID document_uuid;
};

/**
 * An event for panning a document viewport rightward.
 */
struct PanViewportRightEvent final {
  UUID document_uuid;
};

}  // namespace tactile::core
