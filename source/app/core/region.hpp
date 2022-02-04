#pragma once

#include "tile_position.hpp"

namespace tactile {

/**
 * \brief Represents a region of a grid.
 *
 * \ingroup core
 */
struct Region final
{
  tile_position begin;  ///< The top-left position.
  tile_position end;    ///< The bottom-right position.

  [[nodiscard]] auto operator==(const Region&) const noexcept -> bool = default;
};

}  // namespace tactile