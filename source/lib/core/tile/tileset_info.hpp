// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/texture.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

/// Provides information necessary to construct tilesets.
struct TilesetInfo final {
  Shared<Texture> texture;  /// The associated texture.
  Int2 tile_size {};        /// Logical tile size.
};

}  // namespace tactile
