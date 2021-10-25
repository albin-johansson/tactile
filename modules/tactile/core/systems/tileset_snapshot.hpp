#pragma once

#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/components/uv_tile_size.hpp"
#include "property_context_snapshot.hpp"

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