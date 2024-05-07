// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/io/texture.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

/// \addtogroup Tileset
/// \{

/**
 * Represents the information needed to construct a tileset.
 */
struct TilesetSpec final
{
  /** The logical tile size. */
  Int2 tile_size;

  /** The associated texture. */
  CTexture texture;
};

/// \}

}  // namespace tactile
