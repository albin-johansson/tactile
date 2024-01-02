// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Simply represents different mouse buttons.
 */
enum class MouseButton : int8 {
  kLeft,
  kMiddle,
  kRight
};

/**
 * Provides information about a mouse event, used by tool implementations.
 */
struct MouseEvent final {
  MouseButton button;  ///< The active mouse button.
  Float2 position;     ///< The current mouse position.
};

}  // namespace tactile
