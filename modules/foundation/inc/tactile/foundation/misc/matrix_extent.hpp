// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <compare>  // strong_ordering

#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Represents the dimensions of a tile matrix.
 */
struct MatrixExtent final {
  ssize row_count {};  ///< The number of tile rows.
  ssize col_count {};  ///< The number of tile columns.

  [[nodiscard]] constexpr auto operator<=>(const MatrixExtent&) const noexcept
      -> std::strong_ordering = default;
};

}  // namespace tactile