// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/basic/primitives.hpp"

namespace tactile {

/// A component type that represents an index of a tile in a tileset.
struct TACTILE_CORE_API CTileIndex final
{
  /// The tile index.
  usize index;
};

}  // namespace tactile
