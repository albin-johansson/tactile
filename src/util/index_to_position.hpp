#pragma once

#include "map_position.hpp"

namespace tactile {

[[nodiscard]] constexpr auto IndexToPosition(const int index,
                                             const col_t nCols) noexcept
    -> core::MapPosition
{
  return core::MapPosition{row_t{index / nCols.get()}, col_t{index % nCols.get()}};
}

}  // namespace tactile
