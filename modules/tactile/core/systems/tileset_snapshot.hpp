#pragma once

#include "property_context_snapshot.hpp"
#include "tactile/core/components/texture.hpp"
#include "tactile/core/components/tileset.hpp"
#include "tactile/core/components/uv_tile_size.hpp"

namespace Tactile::Sys {

struct TilesetSnapshot final
{
  Tileset core;
  TilesetSelection selection;
  Texture texture;
  UvTileSize uv;
  PropertyContextSnapshot context;
};

}  // namespace Tactile::Sys