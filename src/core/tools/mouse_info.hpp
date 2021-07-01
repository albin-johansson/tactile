#pragma once

#include <centurion.hpp>  // mouse_button

#include "core/map/map_position.hpp"

namespace Tactile {

struct MouseInfo final
{
  MapPosition mouse_position_in_map;
  cen::mouse_button button;
};

}  // namespace Tactile
