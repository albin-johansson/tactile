#pragma once

#include "position.hpp"

namespace tactile {

[[nodiscard]] constexpr auto IndexToPosition(const int index,
                                             const col_t nCols) noexcept
    -> core::Position
{
  return core::Position{row_t{index / nCols.get()}, col_t{index % nCols.get()}};
}

}  // namespace tactile
