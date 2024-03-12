// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/int.hpp"

namespace tactile {

struct TileExtent final {
  usize rows {};
  usize cols {};

  [[nodiscard]] bool operator==(const TileExtent& other) const = default;
};

}  // namespace tactile
