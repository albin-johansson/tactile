// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/io/texture.hpp"

namespace tactile {

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

}  // namespace tactile
