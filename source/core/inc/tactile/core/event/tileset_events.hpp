// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/path.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

/**
 * Event for opening the tileset creation dialog.
 */
struct ShowNewTilesetDialogEvent final
{};

/**
 * Event for adding a tileset to a map.
 */
struct AddTilesetEvent final
{
  /** The path to the tileset texture. */
  Path texture_path;

  /** The size of tiles in the tileset texture. */
  Int2 tile_size;
};

}  // namespace tactile
