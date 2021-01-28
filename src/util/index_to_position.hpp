#pragma once

#include "position.hpp"

namespace tactile {

[[nodiscard]] constexpr auto index_to_position(const int index,
                                               const col_t nCols) noexcept
    -> core::position
{
  return core::position{row_t{index / nCols.get()}, col_t{index % nCols.get()}};
}

}  // namespace tactile
