// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/orthogonal_map.hpp"

namespace tactile {

OrthogonalMap::OrthogonalMap(const MatrixExtent& extent, const Int2& tile_size)
  : AbstractMap {extent, tile_size}
{}

void OrthogonalMap::accept(IMetaContextVisitor& visitor)
{
  visitor.visit(*this);

  for (auto& attached_tileset : mTilesets) {
    attached_tileset.tileset().accept(visitor);
  }

  root_layer().each(visitor);
}

}  // namespace tactile
