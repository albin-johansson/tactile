// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <filesystem>  // path

#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::core {

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
  std::filesystem::path texture_path;

  /** The size of tiles in the tileset texture. */
  Int2 tile_size;
};

}  // namespace tactile::core
