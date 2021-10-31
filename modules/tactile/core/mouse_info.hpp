#pragma once

#include <tactile-base/map_position.hpp>

#include <centurion.hpp>  // mouse_button

namespace Tactile {

/// \addtogroup core
/// \{

struct MouseInfo final
{
  float x{};
  float y{};
  MapPosition position_in_map;
  cen::mouse_button button;
};

/// \} End of group core

}  // namespace Tactile
