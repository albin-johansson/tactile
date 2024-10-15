// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <vector>  // vector

#include "tactile/base/id.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

/**
 * A component representing a tile definition.
 */
struct CTile final
{
  /** The identifier of the tile in the parent tileset (the "local" identifier). */
  TileIndex index;

  /** The embedded objects. */
  std::vector<EntityID> objects;
};

}  // namespace tactile::core
