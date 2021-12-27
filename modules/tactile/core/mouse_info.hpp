#pragma once

#include <centurion.hpp>  // mouse_button

#include "core/map_position.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

struct MouseInfo final {
  float x{};
  float y{};
  MapPosition position_in_map;
  cen::MouseButton button{};
};

/// \} End of group core

}  // namespace Tactile
