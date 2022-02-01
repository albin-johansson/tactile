#pragma once

#include <centurion.hpp>

#include "tile_position.hpp"

namespace tactile {

/**
 * \brief Provides general information about the mouse, used in response to mouse events.
 *
 * \ingroup core
 */
struct mouse_info final
{
  float x{};                           ///< The mouse x-coordinate.
  float y{};                           ///< The mouse y-coordinate.
  tile_position position_in_viewport;  ///< The hovered tile position.
  cen::mouse_button button{};          ///< The activated mouse button.
};

}  // namespace tactile
