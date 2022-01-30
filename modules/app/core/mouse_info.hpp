#pragma once

#include <centurion.hpp>

#include "tile_position.hpp"

namespace tactile {

/// \addtogroup core
/// \{

struct MouseInfo final
{
  float x{};
  float y{};
  tile_position position_in_map;
  cen::mouse_button button{};
};

/// \} End of group core

}  // namespace tactile
