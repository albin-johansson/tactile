#pragma once

#include "core/map_position.hpp"

namespace tactile {

/// \brief Represents a region of a grid.
/// \ingroup core
struct Region final
{
  MapPosition begin;  ///< The top-left position.
  MapPosition end;    ///< The bottom-right position.

  [[nodiscard]] auto operator==(const Region&) const noexcept -> bool = default;
};

}  // namespace tactile