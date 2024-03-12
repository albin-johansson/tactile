// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/tile/tile_pos.hpp"

namespace tactile {

/// Represents a region of a grid.
struct Region final {
  TilePos begin;  /// The top-left position.
  TilePos end;    /// The bottom-right position.

  [[nodiscard]] auto operator==(const Region&) const noexcept -> bool = default;
};

}  // namespace tactile
